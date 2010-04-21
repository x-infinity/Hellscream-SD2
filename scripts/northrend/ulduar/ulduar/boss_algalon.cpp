/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
 
/* ScriptData
SDName: Algalon the Observer
SD%Complete: 60% ?
SDComment: -
SDAuthor: Vaenyr
SDEditor: Darkrulerz
SDCategory: Ulduar
EndScriptData */
 
#include "precompiled.h"
#include "ulduar.h"
#include <map> // for timers in black_holeAI
#include <utility> // for 'make_pair' in black_holeAI

enum Spells
{
    A_CosmicSmash           = 62311,
    A_BlachHoleExplosion    = 64122,
    A_QuantumStrike         = 64395,
    A_PhasePunch            = 64412,
    A_BigBang               = 64443,
    A_AscendToTheHeavens    = 64487,
    A_AlgalonBerserk        = 47008,

    A_BigBang_H             = 64584,
    A_CosmicSmash_H         = 64596,
    A_QuantumStrike_H       = 64592,

    CS_EXPLODION            = 64122,
    CS_EXPLODION_H          = 65108,
    LC_ARCANE_BARRAGE       = 64599,
    LC_ARCANE_BARRAGE_H     = 64607
};

enum Says
{
    Say_Intro1           = -1615172,
    Say_Intro2           = -1615173,
    Say_Intro3           = -1615174,

    Say_FirstTime        = -1615175,

    Say_Aggro            = -1615176,

    Say_SummoningStar    = -1615177,

    Say_BigBang1         = -1615178,
    Say_BigBang2         = -1615179,

    Say_Phase2           = -1615180,

    Say_Kill1            = -1615181,
    Say_Kill2            = -1615182,

    Say_Berserk          = -1615183,

    Say_Despawn1         = -1615184,
    Say_Despawn2         = -1615185,
    Say_Despawn3         = -1615186,

    Say_Defeated1        = -1615187,
    Say_Defeated2        = -1615188,
    Say_Defeated3        = -1615189,
    Say_Defeated4        = -1615190,
    Say_Defeated5        = -1615191
};

enum Defines
{
    STAR                 = 32955,
    CONSTELLATION        = 33052,
    BLACK_HOLE           = 32953,
    DARK_MATTERY         = 33089,
    UNLEASHED_DM         = 34097,

    DISPLAY_CIRCLE       = 11686,
    COSMIC_SMASH_TARGET  = 33104,

    MAX_ADDS_COUNT       = 50,
    MAX_MATTERY_COUNT    = 25,

    FACTION_HOSTILE      = 16,
    FACTION_NEUTRAL      = 189,
    FACTION_FRIENDLY     = 35
};

float Positions[10][4]    =
{
    {-161.508148f, -25.314932f, 409.803619f, 5.675068f },
    {-116.760887f, -64.332687f, 409.802124f, 2.185675f },
    {-156.050476f, -18.543739f, 409.803619f, 5.588679f },
    {-107.364128f, -52.967808f, 409.802124f, 2.577066f },
    {-103.403206f, -28.356491f, 409.803619f, 3.464689f },
    {-149.887238f, -8.486858f, 409.803619f, 5.227403f },
    {-156.943802f, -46.621128f, 409.803619f, 0.450415f },
    {-131.031052f, -37.260998f, 409.802124f, 1.459268f },
    {-106.733093f, -21.638653f, 409.803619f, 3.675313f },
    {-129.364105f, -2.982942f, 409.803619f, 4.515635f }
};

float aPhase2[4][4]        =
{
    {-116.760887f, -64.332687f, 409.802124f, 2.185675f},
    {-131.031052f, -37.260998f, 409.802124f, 1.459268f},
    {-161.508148f, -25.314932f, 409.803619f, 5.675068f},
    {-107.364128f, -52.967808f, 409.802124f, 2.577066f}
};

typedef std::list<uint64> GuidsList;
#define GET_ALGALON (Creature*)Unit::GetUnit((*m_creature), Instance->GetData64(NPC_ALGALON))


/************/
/*** END OF DEFINES ***/
/************/

struct MANGOS_DLL_DECL boss_algalon_the_observerAI : public ScriptedAI
{
	boss_algalon_the_observerAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Instance = ((ScriptedInstance*)pCreature->GetInstanceData());
        (pCreature->GetMap()->IsRegularDifficulty() ? Hero = false : Hero = true);
        pCreature->setFaction(FACTION_NEUTRAL);
        pCreature->GetPosition(start_x, start_y, start_z);
        ResetDialogow();
		Reset();
	}

	// Variables
    ScriptedInstance* Instance;
    bool Hero;
	bool Fight;

    bool IntroStart;
    bool IntroEnd;
    uint32 IntroTimer;
    uint32 IntroText;
    bool EngagedFirstTime;

    bool DespawnStart;
    bool DespawnEnd;
    uint32 DespawnTimer;
    uint32 DespawnText;

    bool DefeatedStart;
    bool DefeatedEnd;
    uint32 DefeatedTimer;
    uint32 DefeatedText;


    uint32 CosmicSmash_Timer;
    bool Target;
    uint64 TargetGUID;
    uint64 Target2GUID;
    uint64 Target3GUID;

    uint32 QuantumStrike_Timer;
    uint32 PhasePunch_Timer;
    uint32 BigBang_Timer;

    uint64 PhasePunch_GUID;
    uint32 PhasePunch_Count;

    bool Phase2;
    uint32 Berserk_Timer;
    bool JustCastedBigBang;
    uint32 BigBangCast_Timer;

    // First Phase
    GuidsList LivingConstellation_GUID;
    GuidsList CollapsingStar_GUID;
    uint32 Constallation_Timer;
    GuidsList BlackHole_GUID;
    uint32 Star_Timer;

    // Second Phase
    GuidsList DarkMattery_GUID;

    float start_x;
    float start_y;
    float start_z;

    // reset
	void Reset()
	{
        if (Instance->GetData(DATA_ALGALON) == DONE)
            return;

        if (m_creature->HasFlag64(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE+UNIT_FLAG_UNK_16+UNIT_FLAG_PASSIVE))
            m_creature->RemoveFlag64(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE+UNIT_FLAG_UNK_16+UNIT_FLAG_PASSIVE);

        SetCombatMovement(false);

		Fight = false;

        CosmicSmash_Timer = 25000;
        Target = false;
        TargetGUID = 0;
        QuantumStrike_Timer = (rand()%3+3)*1000;     // random time between 3 and 5 secs
        PhasePunch_Timer = 15000;
        BigBang_Timer = 90000;

        PhasePunch_GUID = 0;
        PhasePunch_Count = 0;

        JustCastedBigBang = false;
        BigBangCast_Timer = 9000;

        Phase2 = false;
        Berserk_Timer = 360000;
        Constallation_Timer = 50000;

        Star_Timer = 15000;

        /* reset adds */
        DespawnAll();

        ThreatList const& list = m_creature->getThreatManager().getThreatList();
        for (ThreatList::const_iterator i=list.begin(); i!=list.end(); ++i)
        {
            Unit* pP = Unit::GetUnit((*m_creature), (*i)->getUnitGuid());
            //const SpellEntry* bigbang = GetSpellStore()->LookupEntry(64584);
            //if (pP->IsImmunedToSpell(bigbang))
                pP->ApplySpellImmune(A_BigBang, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, false);
        }
	}

    void ResetDialogow()
    {
        if (Instance->GetData(DATA_ALGALON) == DONE)
            return;

        IntroStart = false;
        IntroEnd = false;
        IntroTimer = 500;
        IntroText = 0;
        EngagedFirstTime = false;

        DespawnStart = false;
        DespawnEnd = false;
        DespawnTimer = 500;
        DespawnText = 0;

        DefeatedStart = false;
        DefeatedEnd = false;
        DefeatedTimer = 10000;
        DefeatedText = 0;
    }


    /************************************/
    /************************************/


    // handle functions
    void JustReachedHome()
    {
        if (Instance->GetData(DATA_ALGALON) != DONE)
        {
            Instance->SetData(DATA_ALGALON, FAIL);
            ResetDialogow();
        }
        DespawnAll();
    }

	void Aggro(Unit* pWho)
	{
        if (m_creature->HasFlag64(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE+UNIT_FLAG_UNK_16+UNIT_FLAG_PASSIVE))
            m_creature->RemoveFlag64(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE+UNIT_FLAG_UNK_16+UNIT_FLAG_PASSIVE);

        if (IntroStart && IntroEnd)
            return;

        if (Instance->GetData(DATA_ALGALON) == NOT_STARTED)
        {
            EngagedFirstTime = true;
            IntroStart = true;
            DoStopCombat(false);
        }
        else
        {
            DoScriptText(Say_Aggro, m_creature);
            DoStartCombat();
        }

        Instance->SetData(DATA_ALGALON, IN_PROGRESS);
	}

	void KilledUnit()
	{
        switch(rand()%2)
        {
        case 0: DoScriptText(Say_Kill1, m_creature); break;
        case 1: DoScriptText(Say_Kill2, m_creature); break;
        }
	}

    void JustSummoned(Creature* pC)
    {
        switch(pC->GetEntry())
        {
        case BLACK_HOLE: BlackHole_GUID.push_back(pC->GetGUID()); break;
        case STAR: CollapsingStar_GUID.push_back(pC->GetGUID()); break;
        case CONSTELLATION: LivingConstellation_GUID.push_back(pC->GetGUID()); break;
        case UNLEASHED_DM: DarkMattery_GUID.push_back(pC->GetGUID()); break;
        }
    }

	void DamageTaken(Unit* pBy, uint32& damage)
    {
        if (damage > m_creature->GetHealth())
        {
            damage = 0;
            DoStopCombat(true, true);
        }
    }

	//UpdateAI
	void UpdateAI(const uint32 diff)
	{
        // Dialogues
        UpdateTexts(diff);

        /*******************************/
        /*******************************/


        // AI
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

        //*******************************************
        // Fight will only begin if bool Fight == true (used for boss  monologues)
        //*******************************************
        if (Fight)
        {

        // Check
        if (!Phase2 && (m_creature->GetHealth()*100)/m_creature->GetMaxHealth() <= 20)
        {
            DoScriptText(Say_Phase2, m_creature);
            DoPhase2();
            Phase2 = true;
        }

        BlackHoleDespawnCheck(); // check if living constellation is in black hole - if yes, despawn both

        if (CheckAscendToHeavens() && !DespawnStart)
        {
            DoStopCombat(true, false); // stop combat + DespawnStart = true
            m_creature->CastStop(); // we need to stop casting big bang first
            DoCast(m_creature->getVictim(), A_AscendToTheHeavens);
        }

        if (m_creature->GetHealth()*100/m_creature->GetMaxHealth() < 2 && !DefeatedStart)
        {
            DefeatedStart = true;
            Instance->SetData(DATA_ALGALON, DONE);
            DoStopCombat(true, true);
            DespawnAll();
        }

        // Phase 1
        if (!Phase2)
        {
            if (Star_Timer < diff)
            {
                if (CollapsingStar_GUID.size() < MAX_ADDS_COUNT)
                    SummonCollapsingStar();

                Star_Timer = 60000;
            }else Star_Timer -= diff;

            if (Constallation_Timer < diff)
            {
                if (LivingConstellation_GUID.size() < MAX_ADDS_COUNT)
                    SummonLivingConstellation();
                Constallation_Timer = 50000;
            }else Constallation_Timer -= diff;
        }


        // All Phases

        if (BigBang_Timer < diff)
        {
            if(!JustCastedBigBang)
            {
                DoCast(m_creature->getVictim(), (Hero ? A_BigBang_H : A_BigBang));
                switch(rand()%2)
                {
                case 0: DoScriptText(Say_BigBang1, m_creature); break;
                case 1: DoScriptText(Say_BigBang2, m_creature); break;
                }
                BigBang_Timer = 8000;
                JustCastedBigBang = true;
            }
            else
            {
                // need to remove players form black holes (unaura) after casting big bang
                JustCastedBigBang = false;
            
                ThreatList const& list = m_creature->getThreatManager().getThreatList();
                for (ThreatList::const_iterator i=list.begin(); i!=list.end(); ++i)
                {
                    if (Unit* pU = Unit::GetUnit((*m_creature), (*i)->getUnitGuid()))
                    {
                        pU->RemoveAurasDueToSpell(62169);
                        pU->RemoveAurasDueToSpell(62168);
                        pU->ApplySpellImmune(A_BigBang, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, false);
                    }
                }

                BigBang_Timer = 82000;
            }
        }else BigBang_Timer -= diff;

        if (PhasePunch_Timer < diff)
        {
            DoCast(m_creature->getVictim(), A_PhasePunch);
            if (PhasePunch_GUID == m_creature->getVictim()->GetGUID())
            {
                ++PhasePunch_Count;
                if (PhasePunch_Count >= 5)
                    m_creature->getThreatManager().modifyThreatPercent(m_creature->getVictim(), -100);
            }
            else
            {
                PhasePunch_GUID = m_creature->getVictim()->GetGUID();
                PhasePunch_Count = 0;
            }
            PhasePunch_Timer = 15000;
        }else PhasePunch_Timer -= diff;
		
		Unit* pTemp = SelectUnit(SELECT_TARGET_RANDOM, 0);
		Unit* pTemp2 = SelectUnit(SELECT_TARGET_RANDOM, 0);
		Unit* pTemp3 = SelectUnit(SELECT_TARGET_RANDOM, 0);

        if (CosmicSmash_Timer < diff)
        {
            if (!Target)
            {
                float fX = pTemp->GetPositionX();
                float fY = pTemp->GetPositionY();
                float fZ = pTemp->GetPositionZ();
                float fO = pTemp->GetOrientation();
                Creature* target = m_creature->SummonCreature(COSMIC_SMASH_TARGET, fX, fY, fZ, fO, TEMPSUMMON_MANUAL_DESPAWN, 500);
                TargetGUID = target->GetGUID();

                if (Hero)
                {
                    float fX2 = pTemp2->GetPositionX();
                    float fY2 = pTemp2->GetPositionY();
                    float fZ2 = pTemp2->GetPositionZ();
                    float fO2 = pTemp2->GetOrientation();
                    Creature* target2 = m_creature->SummonCreature(COSMIC_SMASH_TARGET, fX2, fY2, fZ2, fO2, TEMPSUMMON_MANUAL_DESPAWN, 500);
                    Target2GUID = target2->GetGUID();

                    float fX3 = pTemp3->GetPositionX();
                    float fY3 = pTemp3->GetPositionY();
                    float fZ3 = pTemp3->GetPositionZ();
                    float fO3 = pTemp3->GetOrientation();
                    Creature* target3 = m_creature->SummonCreature(COSMIC_SMASH_TARGET, fX3, fY3, fZ3, fO3, TEMPSUMMON_MANUAL_DESPAWN, 500);
                    Target3GUID = target3->GetGUID();

                }// end if (Hero)

                Target = true;
                CosmicSmash_Timer = 4000;
            }//end if (!Target)
            else
            {
                if (Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature), TargetGUID))
                    DoCast(pTemp, (Hero ? A_CosmicSmash_H : A_CosmicSmash), false);

                if (Hero)
                {
                    if (Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature), Target2GUID))
                        DoCast(pTemp, A_CosmicSmash_H, false);
                    if (Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature), Target3GUID))
                        DoCast(pTemp, A_CosmicSmash_H, false);
                }//end if (Hero)

                Target = false;
                CosmicSmash_Timer = 25000;
            }//end else (if !Target)

        }else CosmicSmash_Timer -= diff;

        if (QuantumStrike_Timer < diff)
        {
            DoCast(m_creature->getVictim(), (Hero ? A_QuantumStrike_H : A_QuantumStrike));
            QuantumStrike_Timer = (rand()%3+3)*1000;
        }else QuantumStrike_Timer -= diff;        


        // Attack and berserk
		DoMeleeAttackIfReady();

        if (Berserk_Timer < diff)
        {
            DoCast(m_creature, A_AlgalonBerserk);
            DoScriptText(Say_Berserk, m_creature);
            Berserk_Timer = 360000;
        }else Berserk_Timer -= diff;

        }//end if (Fight)
        else
            m_creature->SetStandFlags(UNIT_STAND_FLAGS_UNK1);
	}

    // Features

    /***************/
    /*** CHECK FUNCTIONS ***/
    /***************/
    bool CheckAscendToHeavens()
    {
        ThreatList const& list = m_creature->getThreatManager().getThreatList();

        for (ThreatList::const_iterator i = list.begin(); i!=list.end(); ++i)
        {
            if (!(Unit::GetUnit((*m_creature), (*i)->getUnitGuid())->HasAura(62169)))
                return false;
        }
        return true;
    }

    void BlackHoleDespawnCheck()
    {
        bool Despawn;
        for (GuidsList::iterator i=LivingConstellation_GUID.begin(); i!=LivingConstellation_GUID.end(); ++i)
        {
            Despawn = false;
            if (Creature* pLC = (Creature*)Unit::GetUnit((*m_creature), (*i)))
            {
                for (GuidsList::iterator j=BlackHole_GUID.begin(); j!=BlackHole_GUID.end(); ++j)
                {
                    Creature* pBH = (Creature*)Unit::GetUnit((*m_creature), (*j));
                    if (pBH && pLC->GetDistance2d(pBH) < 10.0f)
                    {
                        Despawn = true;
                        BlackHole_GUID.erase(j);
                        LivingConstellation_GUID.erase(i);

                        pLC->ForcedDespawn();
                        pBH->ForcedDespawn();
                        break;
                    }
                }

                if (Despawn)
                    break;
            }
        }
        
    }

    void UpdateTexts(const uint32 diff)
    {
        if (IntroStart && !IntroEnd)
        {
            if (0 == IntroText)
                IntroText = 1;

            if (IntroTimer < diff)
            {
                switch(IntroText)
                {
                case 1:
                    DoScriptText(Say_Intro1, m_creature);
                    IntroTimer = 7000;
                    ++IntroText;
                    break;
                case 2:
                    DoScriptText(Say_Intro2, m_creature);
                    IntroTimer = 5000;
                    ++IntroText;
                    break;
                case 3:
                    DoScriptText(Say_Intro3, m_creature);
                    IntroTimer = 10000;
                     ++IntroText;
                    break;

                case 4:
                    DoScriptText(Say_FirstTime, m_creature);
                    IntroEnd = true;
                    DoStartCombat();
                    break;
                }
            }else IntroTimer -= diff;
        }

        if (DespawnStart && !DespawnEnd)
        {
            if (DespawnTimer < diff)
            {
                if (0 == DespawnText)
                    DespawnText = 1;

                switch(DespawnText)
                {
                case 1:
                    DoScriptText(Say_Despawn1, m_creature);
                    DespawnTimer = 11000;
                    ++DespawnText;
                    break;
                case 2:
                    DoScriptText(Say_Despawn2, m_creature);
                    DespawnTimer = 8000;
                    ++DespawnText;
                    break;
                case 3:
                    DoScriptText(Say_Despawn3, m_creature);
                    DespawnTimer = 7000;
                    ++DespawnText;
                    break;
                case 4:
                    DespawnEnd = true;
                    break;
                }

            }else DespawnTimer -= diff;
        }

        if (DefeatedStart &&  !DefeatedEnd)
        {
            if (DefeatedTimer < diff)
            {
                if (0 == DefeatedText)
                {
                    // to do: add chest with loot
                    DefeatedText = 1;
                }

                switch(DefeatedText)
                {
                case 1:
                    DoScriptText(Say_Defeated1, m_creature);
                    DefeatedTimer = 38000;
                    ++DefeatedText;
                    break;
                case 2:
                    DoScriptText(Say_Defeated2, m_creature);
                    DefeatedTimer = 16000;
                    ++DefeatedText;
                    break;
                case 3:
                    DoScriptText(Say_Defeated3, m_creature);
                    DefeatedTimer = 9000;
                    ++DefeatedText;
                    break;
                case 4:
                    DoScriptText(Say_Defeated4, m_creature);
                    DefeatedTimer = 10000;
                    ++DefeatedText;
                    break;
                case 5:
                    DoScriptText(Say_Defeated5, m_creature);
                    DefeatedTimer = 10000;
                    ++DefeatedText;
                    break;
                case 6:
                    DefeatedEnd = true;
                    m_creature->SetVisibility(VISIBILITY_OFF);
                    break;
                }

            }else DefeatedTimer -= diff;
        }
    }
    /***************/
    /*** COMBAT FUNCTIONS ***/
    /***************/
    void DoPhase2()
    {
        AddsDespawn(CollapsingStar_GUID);
        AddsDespawn(LivingConstellation_GUID);
        AddsDespawn(BlackHole_GUID);

        for (int i=0; i<4; ++i)
            Creature* pBH = m_creature->SummonCreature(BLACK_HOLE, aPhase2[i][0], aPhase2[i][1], aPhase2[i][2], aPhase2[i][3], TEMPSUMMON_DEAD_DESPAWN, 500);

    }
    void DoStartCombat()
    {
        m_creature->RemoveFlag64(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE+UNIT_FLAG_UNK_16+UNIT_FLAG_PASSIVE);
        m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
        SetCombatMovement(true);
        Fight = true;
    }

    void DoStopCombat(bool end, bool defeated = false)
    {
        // make Algalon unable to attack and move
        m_creature->SetFlag64(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE+UNIT_FLAG_UNK_16+UNIT_FLAG_PASSIVE);
        SetCombatMovement(false);
        m_creature->GetMotionMaster()->MoveIdle();

        // if players have defeated Algalon
        if (end && defeated)
        {
            m_creature->RemoveAllAttackers();
            m_creature->AttackStop();
            DefeatedStart = true;
            m_creature->setFaction(FACTION_FRIENDLY);
            DespawnAll();
        }
        
        if (end && !defeated)
            DespawnStart = true;

        Fight = false;
    }


    /*****************/
    /*** ADDS FUNCTIONS ***/
    /****************/
    void SummonCollapsingStar()
    {
        DoScriptText(Say_SummoningStar, m_creature);
        uint32 random1;
        uint32 random2;
        uint32 random3;
        uint32 random4;

        do
        {
            random1 = urand(0, 9);
            random2 = urand(0, 9);
            random3 = urand(0, 9);
            random4 = urand(0, 9);
        }
        while
            (random1 != random2
            && random1 != random3
            && random1 != random4
            && random2 != random3
            && random2 != random4
            && random3 != random4);

        float fX1 = Positions[random1][0];
        float fY1 = Positions[random1][1];
        float fZ1 = Positions[random1][2];
        float fO1 = Positions[random1][3];

        float fX2 = Positions[random2][0];
        float fY2 = Positions[random2][1];
        float fZ2 = Positions[random2][2];
        float fO2 = Positions[random2][3]; 

        float fX3 = Positions[random3][0];
        float fY3 = Positions[random3][1];
        float fZ3 = Positions[random3][2];
        float fO3 = Positions[random3][3];

        float fX4 = Positions[random4][0];
        float fY4 = Positions[random4][1];
        float fZ4 = Positions[random4][2];
        float fO4 = Positions[random4][3];

        GuidsList& list = CollapsingStar_GUID;

        if (list.size() < MAX_ADDS_COUNT)
        {
            if (Creature* pAdd = m_creature->SummonCreature(STAR, fX1, fY1, fZ1, fO1, TEMPSUMMON_DEAD_DESPAWN, 500))
            {
                Unit* target;
                do{
                    target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                }while (target->GetTypeId() != TYPEID_PLAYER);
                if (target)
                    pAdd->AI()->AttackStart(target);
            }
        }
        else return;
        if (list.size() < MAX_ADDS_COUNT)
        {
            if (Creature* pAdd = m_creature->SummonCreature(STAR, fX2, fY2, fZ2, fO2, TEMPSUMMON_DEAD_DESPAWN, 500))
            {
                Unit* target;
                do{
                    target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                }while (target->GetTypeId() != TYPEID_PLAYER);
                if (target)
                    pAdd->AI()->AttackStart(target);
            }
        }
        else return;
        if (list.size() < MAX_ADDS_COUNT)
        {
            if (Creature* pAdd = m_creature->SummonCreature(STAR, fX3, fY3, fZ3, fO3, TEMPSUMMON_DEAD_DESPAWN, 500))
            {
                Unit* target;
                do{
                    target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                }while (target->GetTypeId() != TYPEID_PLAYER);
                if (target)
                    pAdd->AI()->AttackStart(target);
            }
        }
        else return;
        if (list.size() < MAX_ADDS_COUNT)
        {
            if (Creature* pAdd = m_creature->SummonCreature(STAR, fX4, fY4, fZ4, fO4, TEMPSUMMON_DEAD_DESPAWN, 500))
            {
                Unit* target;
                do{
                    target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                }while (target->GetTypeId() != TYPEID_PLAYER);
                if (target)
                    pAdd->AI()->AttackStart(target);
            }
        }
        else return;
    }

    void SummonLivingConstellation()
    {
        uint32 random1;
        uint32 random2;
        uint32 random3;

        do
        {
            random1 = urand(0, 9);
            random2 = urand(0, 9);
            random3 = urand(0, 9);
        }
        while
            (random1 != random2
            && random1 != random3
            && random2 != random3);

        float fX1 = Positions[random1][0];
        float fY1 = Positions[random1][1];
        float fZ1 = Positions[random1][2];
        float fO1 = Positions[random1][3];

        float fX2 = Positions[random2][0];
        float fY2 = Positions[random2][1];
        float fZ2 = Positions[random2][2];
        float fO2 = Positions[random2][3]; 

        float fX3 = Positions[random3][0];
        float fY3 = Positions[random3][1];
        float fZ3 = Positions[random3][2];
        float fO3 = Positions[random3][3];

        GuidsList& list = LivingConstellation_GUID;

        if (list.size() < MAX_ADDS_COUNT)
        {
            if (Creature* pAdd = m_creature->SummonCreature(CONSTELLATION, fX1, fY1, fZ1+5, fO1, TEMPSUMMON_DEAD_DESPAWN, 500))
            {
                Unit* target;
                do{
                    target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                }while (target->GetTypeId() != TYPEID_PLAYER);
                if (target)
                    pAdd->AI()->AttackStart(target);
            }
        }
        else return;
        if (list.size() < MAX_ADDS_COUNT)
        {
            if (Creature* pAdd = m_creature->SummonCreature(CONSTELLATION, fX2, fY2, fZ2+5, fO2, TEMPSUMMON_DEAD_DESPAWN, 500))
            {
                Unit* target;
                do{
                    target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                }while (target->GetTypeId() != TYPEID_PLAYER);
                if (target)
                    pAdd->AI()->AttackStart(target);
            }
        }
        else return;
        if (list.size() < MAX_ADDS_COUNT)
        {
            if (Creature* pAdd = m_creature->SummonCreature(CONSTELLATION, fX3, fY3, fZ3+5, fO3, TEMPSUMMON_DEAD_DESPAWN, 500))
            {
                Unit* target;
                do{
                    target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                }while (target->GetTypeId() != TYPEID_PLAYER);
                if (target)
                    pAdd->AI()->AttackStart(target);
            }
        }
        else return;
    }
    void DespawnAll()
    {
        if (!LivingConstellation_GUID.empty())
            AddsDespawn(LivingConstellation_GUID);
        if (!CollapsingStar_GUID.empty())
            AddsDespawn(CollapsingStar_GUID);
        if (!BlackHole_GUID.empty())
            AddsDespawn(BlackHole_GUID);
        if (!DarkMattery_GUID.empty())
            AddsDespawn(DarkMattery_GUID);
    }
    void AddsDespawn(GuidsList& list)
    {
        if (list.empty())
            return;

        for (GuidsList::iterator i=list.begin(); i!=list.end(); ++i)
        {
            if (Creature* pAdd = (Creature*)Unit::GetUnit((*m_creature), (*i)))
                    pAdd->ForcedDespawn();
        }
        list.clear();
    }
};

CreatureAI* GetAI_boss_algalon_the_observer(Creature* pCreature)
{
	return new boss_algalon_the_observerAI(pCreature);
}


struct MANGOS_DLL_DECL mob_collapsing_starAI : public ScriptedAI
{
	mob_collapsing_starAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Instance = ((ScriptedInstance*)pCreature->GetInstanceData());
        (pCreature->GetMap()->IsRegularDifficulty() ? Hero = false : Hero = true);
        pCreature->setFaction(FACTION_HOSTILE);
        pCreature->GetPosition(act_x, act_y, act_z);
		Reset();
	}

	// Variables
	ScriptedInstance* Instance;
    bool Hero;
	bool Fight;
    uint32 LiveTimer; // add loses 1% of his maximum hp every second
    bool Dead;
    uint32 Dead_Timer;

    float act_x;
    float act_y;
    float act_z;

    float next_x;
    float next_y;
    float next_z;

    float last_x;
    float last_y;
    float last_z;

	void Reset()
	{
		Fight = false;
        LiveTimer = 1000; // 1s
        Dead = false;
        Dead_Timer = 1000;
	}

    void DamageTaken(Unit* pBy, uint32& damage)
    {
        if (damage > m_creature->GetHealth())
        {
            Dead = true;
            damage = 0;
        }
    }

    void JustDied(Unit* pKiller)
    {
        Creature* pAlgalon = GET_ALGALON;

        float fX, fY, fZ, fO;
        m_creature->GetPosition(fX, fY, fZ);
        fO = m_creature->GetOrientation();

        Unit* BlackHole = pAlgalon->SummonCreature(BLACK_HOLE, fX, fY, fZ, fO, TEMPSUMMON_DEAD_DESPAWN, 500);
        m_creature->ForcedDespawn();
    }

	//UpdateAI
	void UpdateAI(const uint32 diff)
	{
        if (!Instance)
            m_creature->ForcedDespawn();

        Creature* pAlgalon = GET_ALGALON;

        if (!pAlgalon || !pAlgalon->isAlive())
            m_creature->ForcedDespawn();

        // add always loses hp - no matter what
        if (LiveTimer < diff)
        {
            uint32 OnePercent = m_creature->GetMaxHealth()/100;
            if (OnePercent < m_creature->GetHealth())
                m_creature->SetHealth(m_creature->GetHealth() - OnePercent);
            else
                Dead = true;
            LiveTimer = 1000;
        }else LiveTimer -= diff;

        if (Dead)
        {
            DoCast(pAlgalon->getVictim(), (Hero ? CS_EXPLODION_H : CS_EXPLODION));
            DoCast(m_creature, 62003); // explosion visual
            if (Dead_Timer < diff)
            {
                float fPosX, fPosY, fPosZ;
				m_creature->GetPosition(fPosX, fPosY, fPosZ);

                Unit* BlackHole = pAlgalon->SummonCreature(BLACK_HOLE, fPosX, fPosY, fPosZ, 0, TEMPSUMMON_DEAD_DESPAWN, 500);
                m_creature->ForcedDespawn();
            }else Dead_Timer -= diff;
        }

	    DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_collapsing_star(Creature* pCreature)
{
	return new mob_collapsing_starAI(pCreature);
}


struct MANGOS_DLL_DECL mob_living_constellationAI : public ScriptedAI
{
	mob_living_constellationAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Instance = ((ScriptedInstance*)pCreature->GetInstanceData());
        pCreature->setFaction(FACTION_HOSTILE);
	}

	// Variables
	ScriptedInstance* Instance;
    uint32 ArcaneBarrage_Timer;

    void Reset()
    {
        ArcaneBarrage_Timer = 5000;
    }

	//UpdateAI
	void UpdateAI(const uint32 diff)
	{
		if (!Instance)
            m_creature->ForcedDespawn();

        Creature* pAlgalon = GET_ALGALON;

        if (!pAlgalon->isAlive())
            m_creature->ForcedDespawn();

        if (ArcaneBarrage_Timer < diff)
        {
            DoCast(m_creature->getVictim(), (Instance ? LC_ARCANE_BARRAGE : LC_ARCANE_BARRAGE_H));
            ArcaneBarrage_Timer = 5000;
        }else ArcaneBarrage_Timer -= diff;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_living_constellation(Creature* pCreature)
{
	return new mob_living_constellationAI(pCreature);
}


struct MANGOS_DLL_DECL mob_black_holeAI : public ScriptedAI
{
    mob_black_holeAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        SetCombatMovement(false);
        Instance = (ScriptedInstance*)pCreature->GetInstanceData();
        pCreature->setFaction(FACTION_NEUTRAL); // Algalon's faction
        Reset();
    }

    ScriptedInstance* Instance;
    uint32 Summon_Timer;
    uint32 DarkMattery_Count;
    std::map<uint64, uint32> Timers;

    void Reset()
    {
        Timers.clear();
        m_creature->SetFlag64(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE+UNIT_FLAG_UNK_16+UNIT_FLAG_PASSIVE+UNIT_FLAG_NOT_SELECTABLE);
        Summon_Timer = 30000;
        DarkMattery_Count = 0;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!Instance)
            m_creature->ForcedDespawn();

        Creature* pAlgalon = GET_ALGALON;

        if (!pAlgalon->isAlive())
            m_creature->ForcedDespawn();

        ThreatList const& list = pAlgalon->getThreatManager().getThreatList();
        for (ThreatList::const_iterator i=list.begin(); i!=list.end(); ++i)
        {
            Unit* pP = Unit::GetUnit((*m_creature), (*i)->getUnitGuid());
            std::map<uint64, uint32>::iterator itr = Timers.find(pP->GetGUID()); // lookup for position with player's GUID

            if (isNearestBlackHole(pP))
                if (m_creature->GetDistance2d(pP) < 7.5f)
                {
                    if (itr != Timers.end() && !(dynamic_cast<boss_algalon_the_observerAI*>(pAlgalon->AI())->DespawnStart)) // if 'find' find position
                    {
                        if (itr->second >= 2000)
                        {
                            SpellEntry const* dota = GetSpellStore()->LookupEntry(62169); // Black Hole DoT (~1,5k na sec)
                            SpellEntry const* debuff = GetSpellStore()->LookupEntry(62168); // debuff - visual "ghost" effect

                            if (debuff && !pP->HasAura(62168))
                            {
                                //m_creature->CastSpell(pP, debuff, false);
                                //for (int i=0; i<3; ++i)
                                //{
                                //    if (!dota->Effect[i])
                                //        continue;
                                //    BlackHoleAura* aura = new BlackHoleAura(debuff, i, NULL, pP);
                                //    pP->AddAura(aura);
                                //}

                                /*
                                * if player have this aura and we cast on him next spell
                                * (above) previous aura will be removed (work in both way)
                                */
                            }
                            pP->ApplySpellImmune(A_BigBang, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, true);
                            m_creature->getThreatManager().modifyThreatPercent(pP, -100);

                            Timers.erase(itr); // erase position
                        }// end if (itr->second >= 2000)
                        else if (!(dynamic_cast<boss_algalon_the_observerAI*>(pAlgalon->AI())->DespawnStart))
                            Timers[pP->GetGUID()] = itr->second+diff; //add 'diff' to timer

                    }// end if (Timers.find(pP->GetGUID()))
                    else
                        Timers.insert(std::make_pair(pP->GetGUID(), diff)); // add position for player with 'diff' time

                }// end if (m_creature->GetDistance2d(pP) < 10.0f)
                else if (itr != Timers.end())
                {
                    Timers.erase(itr);

                    if (!pP->HasAura(62169))
                    {
                        pP->ApplySpellImmune(A_BigBang, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, false);
                        pP->RemoveAurasDueToSpell(62168);
                        pP->RemoveAurasDueToSpell(62169);
                    }
                    //else if (!pP->HasAura(62168))
                    //    for (int i=0; i<3; ++i)
                    //    {
                    //        BlackHoleAura* aura = new BlackHoleAura(GetSpellStore()->LookupEntry(62168), i, NULL, pP);
                    //        pP->AddAura(aura);
                    //    }
                }
        }// End for           

        // Phase
        if (pAlgalon->GetHealth()*100/pAlgalon->GetMaxHealth() <= 20)
        {
            if (Summon_Timer < diff)
            {
                if (DarkMattery_Count < MAX_MATTERY_COUNT)
                {
					float fPosX, fPosY, fPosZ;
					m_creature->GetPosition(fPosX, fPosY, fPosZ);
                    Creature* DM = pAlgalon->SummonCreature(UNLEASHED_DM, fPosX, fPosY, fPosZ, 0, TEMPSUMMON_DEAD_DESPAWN, 500);
                    if (DM)
                        DM->AI()->AttackStart(SelectUnit(SELECT_TARGET_RANDOM, 0));
                    ++DarkMattery_Count;
                }
                Summon_Timer = 30000;
            }else Summon_Timer -= diff;
        }
    }


    bool isNearestBlackHole(Unit* pUnit)
    {
        Creature* pAlgalon = GET_ALGALON;
        float MyDis = m_creature->GetDistance2d(pUnit);

        GuidsList const& list = dynamic_cast<boss_algalon_the_observerAI*>(pAlgalon->AI())->BlackHole_GUID;
        for (GuidsList::const_iterator i=list.begin(); i!=list.end(); ++i)
        {
            Creature* pBH = (Creature*)Unit::GetUnit((*m_creature), (*i));
            if (pBH != m_creature)
                if (pUnit->GetDistance2d(pBH) < MyDis)
                    return false;
        }
        return true;
    }
};

CreatureAI* GetAI_mob_black_hole(Creature* pCreature)
{
    return new mob_black_holeAI(pCreature);
}


struct MANGOS_DLL_DECL mob_cosmic_smash_targetAI : public ScriptedAI
{
    mob_cosmic_smash_targetAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        SetCombatMovement(false);
        Instance = (ScriptedInstance*)pCreature->GetInstanceData();
        pCreature->SetVisibility(VISIBILITY_ON);
        Reset();
    }

    ScriptedInstance* Instance;

    void Reset()
    {
        m_creature->SetDisplayId(DISPLAY_CIRCLE);
		m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void UpdateAI(uint32 const diff)
    {
        if (!Instance)
            m_creature->ForcedDespawn();

        Creature* pAlgalon = GET_ALGALON;

        if (!pAlgalon || !pAlgalon->isAlive())
            m_creature->ForcedDespawn();

        if (!pAlgalon->getVictim() || !pAlgalon->SelectHostileTarget())
            m_creature->ForcedDespawn();

        if (!m_creature->getVictim() || m_creature->SelectHostileTarget())
            m_creature->ForcedDespawn();
    }
};
CreatureAI* GetAI_mob_cosmic_smash_target(Creature* pCreature)
{
    return new mob_cosmic_smash_targetAI(pCreature);
}


struct MANGOS_DLL_DECL mob_dark_matteryAI : public ScriptedAI
{
    mob_dark_matteryAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Instance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* Instance;

    bool Anyone;
    std::vector<uint64> Targets;

    void Reset()
    {
        Anyone = false;
        Targets.clear();
    }

    void UpdateAI(const uint32 diff)
    {
        if (!Instance)
            m_creature->ForcedDespawn();

        Creature* pAlgalon = GET_ALGALON;
        if (!pAlgalon || !pAlgalon->isAlive())
            m_creature->ForcedDespawn();
        if (!pAlgalon->getVictim() || !pAlgalon->SelectHostileTarget())
            m_creature->ForcedDespawn();
        if (!m_creature->getVictim() || !m_creature->SelectHostileTarget())
            m_creature->ForcedDespawn();

        ThreatList const& list = pAlgalon->getThreatManager().getThreatList();

        Anyone = false;
        Targets.clear();
        for (ThreatList::const_iterator i=list.begin(); i!=list.end(); ++i)
        {
            if (Unit* pP = Unit::GetUnit((*m_creature), (*i)->getUnitGuid()))
                if (pP->HasAura(62168) && pP->isAlive())
                {
                    Anyone = true;
                    Targets.push_back(pP->GetGUID());
                }
        }

        if (Anyone)
        {
            uint32 random = urand(0, Targets.size()-1);
            if (Unit* pTemp = Unit::GetUnit((*m_creature), Targets[random]))
                m_creature->AI()->AttackStart(pTemp);
        }
        else
        {
            m_creature->AttackStop();
            m_creature->CombatStop(true);
            m_creature->GetMotionMaster()->MoveIdle();
        }
    }
};
CreatureAI* GetAI_mob_dark_mattery(Creature* pCreature)
{
    return new mob_dark_matteryAI(pCreature);
}

/***********************************************************/

void AddSC_boss_algalon()
{
	Script* newscript;

	newscript = new Script;
	newscript->Name = "boss_algalon_the_observer";
	newscript->GetAI = &GetAI_boss_algalon_the_observer;
	newscript->RegisterSelf();

	newscript = new Script;
	newscript->Name = "mob_collapsing_star";
	newscript->GetAI = &GetAI_mob_collapsing_star;
	newscript->RegisterSelf();

	newscript = new Script;
	newscript->Name = "mob_living_constellation";
	newscript->GetAI = &GetAI_mob_living_constellation;
	newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_black_hole";
    newscript->GetAI = &GetAI_mob_black_hole;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_cosmic_smash_target";
    newscript->GetAI = &GetAI_mob_cosmic_smash_target;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_dark_mattery";
    newscript->GetAI = &GetAI_mob_dark_mattery;
    newscript->RegisterSelf();
}