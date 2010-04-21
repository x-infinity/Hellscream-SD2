#include "precompiled.h"
#include "instance_trial_of_the_crusader.h"


struct MANGOS_DLL_DECL npc_dreadscaleAI : public ScriptedAI
{
npc_dreadscaleAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}
//Timers
uint32 BurningBit_Timer;
uint32 MoltenSpew_Timer;
uint32 SlimePool_Timer;
uint32 FireSpit_Timer;
uint32 BurningSpray_Timer;
uint32 Sweep_Timer;
uint32 flag;


	void Reset()
{

BurningBit_Timer = 12000;
MoltenSpew_Timer = 8000;
SlimePool_Timer = 17000;
FireSpit_Timer = 2500;
BurningSpray_Timer = 6000;
Sweep_Timer = 9000;


}

void Aggro(Unit *who)
	{
	if(flag == 1)
	{
	}else{

	Unit *pTarget1 = SelectUnit(SELECT_TARGET_RANDOM,0);
		m_creature->MonsterSay("Agro'ed!", LANG_UNIVERSAL, 0);
		Creature *pAcidmaw = m_creature->SummonCreature(Acidmaw,538.347, 165.509, 394.6500, 5.431659,TEMPSUMMON_MANUAL_DESPAWN, 0);
		pAcidmaw->HandleEmoteCommand(Emerge);
		pAcidmaw->AI()->AttackStart(pTarget1); 
		flag = 1;
		pAcidmaw->GetMotionMaster()->Clear(false);
		Unit *pTarget = SelectUnit(SELECT_TARGET_RANDOM,0);
		pAcidmaw->AI()->AttackStart(pTarget); 

	}
	}

void UpdateAI(const uint32 diff )
	{
		ScriptedInstance* pInstance = ((ScriptedInstance*)m_creature->GetInstanceData());

	if(pInstance && pInstance->GetData(101) == HIDING)
	{
	m_creature->GetMotionMaster()->Clear(true);
	} else {

			//BEGIN AI

				//BOSS 1
			if(pInstance && pInstance->GetData(100) == SWAP_Orginal)
			{
				if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
					return;
			//************MOBILE********//
				//burning bite
					if(BurningBit_Timer < diff)
					{
						m_creature->MonsterSay("BurningBit", LANG_UNIVERSAL, 0);
						DoCast(m_creature->getVictim(), BurningBit);
						BurningBit_Timer = 12000; //change
					}else BurningBit_Timer -= diff;
				//molten spew
					if(MoltenSpew_Timer < diff)
					{
						m_creature->MonsterSay("MoltenSpew", LANG_UNIVERSAL, 0);
						DoCast(m_creature->getVictim(), MoltenSpew);
						MoltenSpew_Timer = 8000; //change
					}else MoltenSpew_Timer -= diff;
				//slime pool
					if(SlimePool_Timer < diff)
					{
						m_creature->MonsterSay("SlimePool", LANG_UNIVERSAL, 0);
						DoCast(m_creature->getVictim(), SlimePool);
						SlimePool_Timer = 18000; //change
					}else SlimePool_Timer -= diff;
			}else{
			//************STATIONARY********//
				if (!m_creature->SelectHostileTarget())
					return;
				m_creature->GetMotionMaster()->Clear(false);
				// fire spit
					if(FireSpit_Timer < diff)
					{
						m_creature->MonsterSay("FireSpit", LANG_UNIVERSAL, 0);
						DoCast(m_creature->getVictim(), FireSpit);
						FireSpit_Timer = 3000; //change
					}else FireSpit_Timer -= diff;
				//burning spray
					if(BurningSpray_Timer < diff)
					{
						m_creature->MonsterSay("BurningSpray", LANG_UNIVERSAL, 0);
						DoCast(m_creature->getVictim(), BurningSpray);
						BurningSpray_Timer = 6000; //change
					}else BurningSpray_Timer -= diff;
				//sweep
					if(Sweep_Timer < diff)
					{
						m_creature->MonsterSay("Sweep", LANG_UNIVERSAL, 0);
						DoCast(m_creature->getVictim(), Sweep);
						Sweep_Timer = 9000; //change
					}else Sweep_Timer -= diff;
			}
				

			      DoMeleeAttackIfReady();
		}
	}
};


struct MANGOS_DLL_DECL npc_acidmawAI : public ScriptedAI
{
npc_acidmawAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

	uint32 ParalyticSpray_Timer;
	uint32 AcidSpit_Timer;
	uint32 Sweep_Timer;
	uint32 ParalyticBite_Timer;
	uint32 AcidicSpew_Timer;
	uint32 SlimePool_Timer;
		
	void Reset()
{
	ParalyticSpray_Timer = 14500;
	AcidSpit_Timer = 3000;
	Sweep_Timer = 6000;
	ParalyticBite_Timer = 9500;
	AcidicSpew_Timer = 10000;
	SlimePool_Timer = 15000;
}

void Aggro(Unit *who)
	{
		m_creature->MonsterSay("Me Hungry!", LANG_UNIVERSAL, 0);
	}

void UpdateAI(const uint32 diff )
	{
		ScriptedInstance* pInstance = ((ScriptedInstance*)m_creature->GetInstanceData());
	if(pInstance && pInstance->GetData(101) == HIDING)
	{
		m_creature->GetMotionMaster()->Clear(true);
	} else {
			//BEGIN AI

				//BOSS 1
			if(pInstance && pInstance->GetData(100) == SWAP_Orginal)
			{
						//************STATIONARY********//
				if (!m_creature->SelectHostileTarget())
					return;
				m_creature->GetMotionMaster()->Clear(false);
				// ParalyticSpray
					if(ParalyticSpray_Timer < diff)
					{
						m_creature->MonsterSay("ParalyticSpray", LANG_UNIVERSAL, 0);
						DoCast(m_creature->getVictim(), ParalyticSpray);
						ParalyticSpray_Timer = 7500; //change
					}else ParalyticSpray_Timer -= diff;
				//AcidSpit
					if(AcidSpit_Timer < diff)
					{
						m_creature->MonsterSay("AcidSpit", LANG_UNIVERSAL, 0);
						DoCast(m_creature->getVictim(), AcidSpit);
						AcidSpit_Timer = 2000; //change
					}else AcidSpit_Timer -= diff;
				//sweep
					if(Sweep_Timer < diff)
					{
						m_creature->MonsterSay("Sweep", LANG_UNIVERSAL, 0);
						DoCast(m_creature->getVictim(), Sweep);
						Sweep_Timer = 6000; //change
					}else Sweep_Timer -= diff;
			}else{
					//************MOBILE********//
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;
				//ParalyticBite
					if(ParalyticBite_Timer < diff)
					{
						m_creature->MonsterSay("ParalyticBite", LANG_UNIVERSAL, 0);
						DoCast(m_creature->getVictim(), ParalyticBite);
						ParalyticBite_Timer = 4500; //change
					}else ParalyticBite_Timer -= diff;
				//AcidicSpew
					if(AcidicSpew_Timer < diff)
					{
						m_creature->MonsterSay("AcidicSpew", LANG_UNIVERSAL, 0);
						DoCast(m_creature->getVictim(), AcidicSpew);
						AcidicSpew_Timer = 3000; //change
					}else AcidicSpew_Timer -= diff;
				//slime pool
					if(SlimePool_Timer < diff)
					{
						m_creature->MonsterSay("SlimePool", LANG_UNIVERSAL, 0);
						DoCast(m_creature->getVictim(), SlimePool);
						SlimePool_Timer = 6000; //change
					}else SlimePool_Timer -= diff;

			}
				

				DoMeleeAttackIfReady();
		}
	}
};


CreatureAI* GetAI_npc_dreadscale(Creature* pCreature)
{
    return new npc_dreadscaleAI(pCreature);
}

CreatureAI* GetAI_npc_acidmaw(Creature* pCreature)
{
    return new npc_acidmawAI(pCreature);
}

void AddSC_the_two_jormungar()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "npc_dreadscale";
    newscript->GetAI = &GetAI_npc_dreadscale;
    newscript->RegisterSelf();
	
    newscript = new Script;
    newscript->Name = "npc_acidmaw";
    newscript->GetAI = &GetAI_npc_acidmaw;
    newscript->RegisterSelf();
}