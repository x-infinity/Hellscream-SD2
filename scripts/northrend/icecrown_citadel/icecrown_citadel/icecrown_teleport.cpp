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
SDName: icecrown_teleport
SD%Complete: 30%
SDComment: by /dev/rsa
SDCategory: Icecrown Citadel
EndScriptData */
#include "precompiled.h"
#include "def_spire.h"

enum 
{
	PORTALS_COUNT = 8
};

struct t_Locations
{
    char const* name;
    float x, y, z;
    uint32 id;
    bool state;
    bool active;
    uint32 encounter;
};

static t_Locations PortalLoc[]=
{
{"Hammer light",-17.1928f, 2211.44f, 30.1158f,0.0f,true,true,TYPE_TELEPORT}, //
{"Spire",-503.62f, 2211.47f, 62.8235f,1.0f,false,true,TYPE_MARROWGAR},  //
{"Cephalic shaft",-615.145f, 2211.47f, 199.972f,2.0f,false,true,TYPE_DEATHWHISPER}, //
{"Air battle",-209.5f, 2211.91f, 199.97f,3.0f,false,true,TYPE_SKULLS_PLATO}, //
{"The rise of deadly",-549.131f, 2211.29f, 539.291f,4.0f,false,true,TYPE_FLIGHT_WAR}, //
{"Ice Crown Citadel",4198.42f, 2769.22f, 351.065f,5.0f,false,false,TYPE_SAURFANG}, //
{"Sanctuary blood",4490.205566f, 2769.275635f, 403.983765f,6.0f,false,false,TYPE_BLOOD_COUNCIL}, //
{"Lair of the Queen of Ice",4356.236816f, 2402.573242f, 220.462540f,7.0f,false,false,TYPE_VALITHRIA}, //
};


bool GossipSelect_icecrown_teleporter(Player *player, Creature* pCreature, uint32 sender, uint32 action)
{
    if(sender != GOSSIP_SENDER_MAIN) return true;
    if(!player->getAttackers().empty()) return true;
    if(action >= 0 && action <= PORTALS_COUNT)
        player->TeleportTo(MAP_NUM, PortalLoc[action].x, PortalLoc[action].y, PortalLoc[action].z, 0);
        player->CLOSE_GOSSIP_MENU();
    return true;
}

bool GossipHello_icecrown_teleporter(Player *player, Creature* pCreature)
{
    ScriptedInstance *pInstance = (ScriptedInstance *) pCreature->GetInstanceData();
    if(!pInstance) return true;
    bool m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();

    for(uint8 i = 0; i < PORTALS_COUNT; i++) {
    if (PortalLoc[i].active == true && (PortalLoc[i].state == true || pInstance->GetData(TYPE_TELEPORT) >= PortalLoc[i].encounter))
             player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, PortalLoc[i].name, GOSSIP_SENDER_MAIN, i);
    };
    player->SEND_GOSSIP_MENU(TELEPORT_GOSSIP_MESSAGE, pCreature->GetGUID());
    return true;
}


bool GOHello_go_icecrown_teleporter(Player *player, GameObject* pGo)
{

    ScriptedInstance *pInstance = (ScriptedInstance *) pGo->GetInstanceData();
    if(!pInstance) return true;

    bool m_bIsRegularMode = pGo->GetMap()->IsRegularDifficulty();

    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, "No message", GOSSIP_SENDER_MAIN, 0);
    player->SEND_GOSSIP_MENU(GO_TELEPORT_GOSSIP_MESSAGE, pGo->GetGUID());
    return true;
}


void AddSC_icecrown_teleporter()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "icecrown_teleporter";
    newscript->pGossipHello = &GossipHello_icecrown_teleporter;
    newscript->pGossipSelect = &GossipSelect_icecrown_teleporter;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_icecrown_teleporter";
    newscript->pGOHello = &GOHello_go_icecrown_teleporter;
    newscript->RegisterSelf();
}
