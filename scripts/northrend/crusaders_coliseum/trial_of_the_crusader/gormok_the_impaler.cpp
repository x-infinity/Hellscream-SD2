#include "precompiled.h"
#include "instance_trial_of_the_crusader.h"

struct MANGOS_DLL_DECL gormok_the_impalerAI : public ScriptedAI
{
    //*** HANDLED FUNCTION ***
    //This is the constructor, called only once when the creature is first created
    gormok_the_impalerAI(Creature* pCreature) : ScriptedAI(pCreature) {
    //    m_uiNpcEntry = pCreature->GetEntry();
        Reset();
    }


	//defining my shit
	uint32 Phase;
	uint32 stomp_timer;
	uint32 impale_timer;
	uint32 whirl_timer;
	uint32 PhaseTimer;
	uint32 EndPhaseTwo;
	uint32 summon_timer;
	uint32 summon_count;


void Reset()
{
	Phase = 0;
	PhaseTimer = 60000; // 2 minute
	stomp_timer = 10000; //30secs for spell 1
	impale_timer = 5000; // 5 seconds 
	summon_timer = 20000; // 25 seconds
	whirl_timer = 5000;
	summon_count = 0;
}

void Aggro(Unit *who)
	{
 m_creature->MonsterSay("ROAR!", LANG_UNIVERSAL, 0);
	}

void UpdateAI(const uint32 diff )
	{
	
		//Return since we have no target
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

			//BEGIN AI
			
				if(stomp_timer < diff)
				{
					m_creature->MonsterSay("I STOMP ON YOUR FACE!", LANG_UNIVERSAL, 0);
					DoCast(m_creature->getVictim(),stomp);
					stomp_timer = 30000;
				}else stomp_timer -= diff;
				
				if(impale_timer < diff)
				{	
					DoCast(m_creature->getVictim(),impale);
					impale_timer = 10000;
					m_creature->MonsterSay("IMPALE!", LANG_UNIVERSAL, 0);
				}else impale_timer -= diff;
				

				if(summon_timer < diff && summon_count <= 4)
				{
					Unit *pTarget1 = SelectUnit(SELECT_TARGET_RANDOM,0);
					Creature *pLarva1 = pTarget1->SummonCreature(34800,pTarget1->GetPositionX() ,pTarget1->GetPositionY()+1, pTarget1->GetPositionZ()+1, 0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 3000);
					DoCast(m_creature,RisingAnger);	
					//CastSpell(m_creature,RisingAnger,false);
					pLarva1->AI()->AttackStart(pTarget1); 
					pLarva1->MonsterSay("Boss Enjoy the buff :D", LANG_UNIVERSAL, 0);
					summon_count = (summon_count + 1);

				summon_timer = 60000;
					m_creature->MonsterSay("Enjoy my little friends", LANG_UNIVERSAL, 0);
				}else summon_timer -= diff;

		
			      DoMeleeAttackIfReady();
	}

	    void JustDied(Unit *killer)
    {
	ScriptedInstance* pInstance = ((ScriptedInstance*)m_creature->GetInstanceData());
			pInstance->SetData(DOOR1, NOT_STARTED);
            pInstance->SetData(BOSS1, DONE);
			pInstance->SetData(BOSS2, IN_PROGRESS);
    } 
};


CreatureAI* GetAI_gormok_the_impaler(Creature* pCreature)
{
    return new gormok_the_impalerAI(pCreature);
}



void AddSC_gormok_the_impaler()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "gormok_the_impaler";
    newscript->GetAI = &GetAI_gormok_the_impaler;
    newscript->RegisterSelf();
}