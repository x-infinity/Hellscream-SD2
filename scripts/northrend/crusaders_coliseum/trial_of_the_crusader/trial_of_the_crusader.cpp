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
SDName: trial_of_the_crusader
SD%Complete: 0
SDComment:
SDCategory: Crusader Coliseum
EndScriptData */

#include "precompiled.h"
#include "trial_of_the_crusader.h"

#define GOSSIP_START_EVENT1 "Yes! We are ready to face the challenges ahead."

/*######
## npc_crusader_anouncer
######*/

struct MANGOS_DLL_DECL npc_crusader_anouncerAI : public ScriptedAI
{
    npc_crusader_anouncerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    void Reset() {}

    void StartEvent(Player* pPlayer) // Summon start
    {
        if(!m_pInstance)
            return;

        if(m_pInstance->GetData(TYPE_NORTHREND_BEASTS) == NOT_STARTED && m_pInstance->GetData(TYPE_ICE) == NOT_STARTED && m_pInstance->GetData(TYPE_ANGELS) == NOT_STARTED && m_pInstance->GetData(TYPE_JARAXXUS) == NOT_STARTED && m_pInstance->GetData(TYPE_ANUBARAK) == NOT_STARTED)
        {
            m_creature->SummonCreature(NPC_SCHLUND, 580.457764f, 177.184662f, 395.136548f, 1.565907f, TEMPSUMMON_DEAD_DESPAWN, 0);
            m_creature->SummonCreature(NPC_GORMOK, 563.768555f, 179.513672f, 394.398224f, 4.687868f, TEMPSUMMON_DEAD_DESPAWN, 0);
            m_creature->SummonCreature(NPC_MAUL, 544.791504f, 174.521866f, 394.679626f, 5.363310f, TEMPSUMMON_DEAD_DESPAWN, 0);
            m_pInstance->SetData(TYPE_NORTHREND_BEASTS, IN_PROGRESS);
        }

        if(m_pInstance->GetData(TYPE_NORTHREND_BEASTS) == DONE && m_pInstance->GetData(TYPE_ICE) == NOT_STARTED && m_pInstance->GetData(TYPE_ANGELS) == NOT_STARTED && m_pInstance->GetData(TYPE_JARAXXUS) == NOT_STARTED && m_pInstance->GetData(TYPE_ANUBARAK) == NOT_STARTED)
        {
            m_creature->SummonCreature(NPC_ICE, 563.768555f, 179.513672f, 394.398224f, 4.687868f, TEMPSUMMON_DEAD_DESPAWN, 0);
            m_pInstance->SetData(TYPE_ICE, IN_PROGRESS);
        }

        if(m_pInstance->GetData(TYPE_NORTHREND_BEASTS) == DONE && m_pInstance->GetData(TYPE_ICE) == DONE && m_pInstance->GetData(TYPE_ANGELS) == NOT_STARTED && m_pInstance->GetData(TYPE_JARAXXUS) == NOT_STARTED && m_pInstance->GetData(TYPE_ANUBARAK) == NOT_STARTED)
        {
	        m_creature->SummonCreature(NPC_HOLY, 580.457764f, 177.184662f, 395.136548f, 1.565907f, TEMPSUMMON_MANUAL_DESPAWN, 0);
            m_creature->SummonCreature(NPC_SHADOW, 544.791504f, 174.521866f, 394.679626f, 5.363310f, TEMPSUMMON_MANUAL_DESPAWN, 0);
		    m_pInstance->SetData(TYPE_ANGELS, IN_PROGRESS);
        }

        if(m_pInstance->GetData(TYPE_NORTHREND_BEASTS) == DONE && m_pInstance->GetData(TYPE_ICE) == DONE && m_pInstance->GetData(TYPE_ANGELS) == DONE && m_pInstance->GetData(TYPE_JARAXXUS) == NOT_STARTED && m_pInstance->GetData(TYPE_ANUBARAK) == NOT_STARTED)
        {
	        m_creature->SummonCreature(NPC_JARU, 563.768555f, 179.513672f, 394.398224f, 4.687868f, TEMPSUMMON_MANUAL_DESPAWN, 0);
	        m_pInstance->SetData(TYPE_JARAXXUS, IN_PROGRESS);
        }

        if(m_pInstance->GetData(TYPE_NORTHREND_BEASTS) == DONE && m_pInstance->GetData(TYPE_ICE) == DONE && m_pInstance->GetData(TYPE_ANGELS) == DONE && m_pInstance->GetData(TYPE_JARAXXUS) == DONE && m_pInstance->GetData(TYPE_ANUBARAK) == NOT_STARTED)
        {
	        m_creature->SummonCreature(NPC_ANUB,  563.768555f, 179.513672f, 394.398224f, 4.687868f, TEMPSUMMON_MANUAL_DESPAWN, 0);
	        m_pInstance->SetData(TYPE_ANUBARAK, IN_PROGRESS);
        }
    }
};

CreatureAI* GetAI_npc_crusader_anouncer(Creature* pCreature)
{
    return new npc_crusader_anouncerAI(pCreature);
}

bool GossipHello_npc_crusader_anouncer(Player* pPlayer, Creature* pCreature)
{
    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_START_EVENT1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_crusader_anouncer(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{ 
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        ((npc_crusader_anouncerAI*)pCreature->AI())->StartEvent(pPlayer);
    }

    return true;
}

void AddSC_trial_of_the_crusader()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "npc_crusader_anouncer";
    newscript->GetAI = &GetAI_npc_crusader_anouncer;
    newscript->pGossipHello = &GossipHello_npc_crusader_anouncer;
    newscript->pGossipSelect = &GossipSelect_npc_crusader_anouncer;
    newscript->RegisterSelf();
}