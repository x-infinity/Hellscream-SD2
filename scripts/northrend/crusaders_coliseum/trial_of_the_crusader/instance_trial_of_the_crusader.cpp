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
SDName: instance_trial_of_the_crusader
SD%Complete: 100
SDComment:
SDCategory: Crusader Coliseum
EndScriptData */

#include "precompiled.h"
#include "trial_of_the_crusader.h"

/* Trial Of The Crusader encounters:
0 - Northrend Beasts
1 - Jaraxxus
2 - Faction Champions
3 - Twin Valkyr
4 - Anubarak
*/

struct MANGOS_DLL_DECL instance_trial_of_the_crusader : public ScriptedInstance 
{ 
    instance_trial_of_the_crusader(Map* pMap) : ScriptedInstance(pMap) { Initialize(); } 
 
    uint32 m_auiEncounter[MAX_ENCOUNTER]; 
    std::string strInstData; 
 
    uint64 m_uiHolyGUID; 
    uint64 m_uiShadowGUID; 
 
    uint64 m_uiJaruGUID; 
 
    uint64 m_uiAnubGUID; 
 
    uint64 m_uiIceGUID; 
 
    uint64 m_uiJaronGUID; 
    uint64 m_uiArelosGUID; 
    uint64 m_uiAnnounceGUID; 
 
    uint64 m_uiAngel; 
    uint64 m_uiBeast; 
    uint64 m_uiGormokGUID; 
    uint64 m_uiSchlundGUID; 
    uint64 m_uiMaulGUID; 
 
    uint64 m_uiChampionsLootGUID; 
    uint64 m_uiShadowLootGUID; 
    uint64 m_uiHolyLootGUID;

    uint64 m_uiDoorGuid;
    uint64 m_uiFloodGuid;
 
    void Initialize() 
    { 
        m_uiChampionsLootGUID   = 0; 
        m_uiHolyGUID            = 0; 
        m_uiHolyLootGUID        = 0; 
        m_uiShadowGUID          = 0; 
        m_uiShadowLootGUID      = 0; 
        m_uiJaruGUID            = 0; 
 
        m_uiAnubGUID            = 0; 
 
        m_uiIceGUID             = 0; 
 
        m_uiJaronGUID           = 0; 
        m_uiArelosGUID          = 0; 
        m_uiAnnounceGUID        = 0; 
 
        m_uiAngel               = 0; 
        m_uiBeast               = 0; 
        m_uiGormokGUID          = 0; 
        m_uiSchlundGUID         = 0; 
        m_uiMaulGUID            = 0;

        m_uiDoorGuid            = 0;
        m_uiFloodGuid           = 0;
 
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter)); 
    } 
 
    bool IsEncounterInProgress() const 
    { 
        for(uint8 i = 0; i < MAX_ENCOUNTER; ++i) 
        { 
            if (m_auiEncounter[i] == IN_PROGRESS) 
                return true; 
        } 
 
        return false; 
    } 
 
    void OnCreatureCreate(Creature* pCreature, bool bAdd) 
    { 
         switch(pCreature->GetEntry()) 
         { 
             // BEASTS! 
             case NPC_GORMOK: 
                 m_uiGormokGUID = pCreature->GetGUID(); 
                 m_uiBeast = pCreature->GetGUID(); 
                 break; 
             case NPC_SCHLUND: 
                 m_uiSchlundGUID = pCreature->GetGUID(); 
                 m_uiBeast = pCreature->GetGUID(); 
                 break; 
             case NPC_MAUL: 
                 m_uiMaulGUID = pCreature->GetGUID(); 
                 m_uiBeast = pCreature->GetGUID(); 
                 break; 
  
                 //EISHEULER 
             case NPC_ICE: 
                 m_uiIceGUID = pCreature->GetGUID(); 
                 break; 
    
             // Angels! 
             case NPC_FJOLA: 
                 m_uiHolyGUID = pCreature->GetGUID();
                 break; 
             case NPC_EIDYS: 
                 m_uiShadowGUID = pCreature->GetGUID();
                 break; 
  
         // Jaruxxus 
             case NPC_JARU: 
                 m_uiJaruGUID = pCreature->GetGUID(); 
                 break; 
  
         // ANUB 
             case NPC_ANUB: 
                 m_uiAnubGUID = pCreature->GetGUID(); 
                 break; 
  
         //Announcers 
             case NPC_JAERON: 
                 m_uiJaronGUID = pCreature->GetGUID(); 
                 break; 
             case NPC_ARELOS: 
                 m_uiArelosGUID = pCreature->GetGUID(); 
                 break; 
         } 
     } 
  
     void OnObjectCreate(GameObject* pGo) 
     { 
         switch(pGo->GetEntry()) 
        { 
         case GO_CHAMPIONS_LOOT: 
            m_uiChampionsLootGUID = pGo->GetGUID();
            break; 
         case GO_Holy_LOOT: 
            m_uiHolyLootGUID = pGo->GetGUID();
            break; 
         case GO_Shadow_LOOT: 
            m_uiShadowLootGUID = pGo->GetGUID();
            break; 
         case GO_CHAMPIONS_LOOT_H: 
            m_uiChampionsLootGUID = pGo->GetGUID();
            break; 
         case GO_Holy_LOOT_H: 
            m_uiHolyLootGUID = pGo->GetGUID();
            break; 
         case GO_Shadow_LOOT_H: 
            m_uiShadowLootGUID = pGo->GetGUID();
            break; 
         case GO_DOOR:
            m_uiDoorGuid = pGo->GetGUID();
            break;
         case GO_FLOOR:
            m_uiFloodGuid = pGo->GetGUID();
         } 
     } 
  
     void SetData(uint32 Type, uint32 Data) 
     { 
         switch(Type) 
         { 
            case TYPE_NORTHREND_BEAST: 
                DoUseDoorOrButton(m_uiDoorGuid);
                if (Data == DONE) {
                    DoUseDoorOrButton(m_uiDoorGuid);
                    m_auiEncounter[0] = Data;
                }
                break; 
            case TYPE_ICE:
                DoUseDoorOrButton(m_uiDoorGuid);
                if (Data == DONE) {
                    DoUseDoorOrButton(m_uiDoorGuid);
                    m_auiEncounter[1] = Data;
                }
                break; 
            case TYPE_ANGELS: 
                DoUseDoorOrButton(m_uiDoorGuid);
                if (Data == DONE) {
                    DoUseDoorOrButton(m_uiDoorGuid);
                    m_auiEncounter[2] = Data;
                }
                break; 
            case TYPE_JARAXXUS: 
                DoUseDoorOrButton(m_uiDoorGuid);
                if (Data == DONE) {
                    DoUseDoorOrButton(m_uiDoorGuid);
                    m_auiEncounter[3] = Data; 
                }
                break; 
            case TYPE_ANUBARAK:
                //GameObject* pFloor = instance->GetGameObject(m_uiFloodGuid);
                DoUseDoorOrButton(m_uiDoorGuid);
                if (Data == DONE) {
                    DoUseDoorOrButton(m_uiDoorGuid);
                    m_auiEncounter[4] = Data;
                }
                break; 
         } 
  
         if (Data == DONE) 
         { 
             OUT_SAVE_INST_DATA; 
  
             std::ostringstream saveStream; 
             saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " " << m_auiEncounter[3] << " " << m_auiEncounter[4]; 
  
             strInstData = saveStream.str(); 
  
             SaveToDB(); 
             OUT_SAVE_INST_DATA_COMPLETE; 
         } 
     } 
  
     uint64 GetData64(uint32 uiData) 
     { 
         switch(uiData) 
         { 
             case NPC_JARU: 
                return m_uiJaruGUID; 
             case NPC_GORMOK:
                 return m_uiGormokGUID;
             case NPC_SCHLUND:
                 return m_uiSchlundGUID;
             case NPC_MAUL:
                 return m_uiMaulGUID;
             case NPC_FJOLA:
                 return m_uiHolyGUID;
             case NPC_EIDYS:
                 return m_uiShadowGUID;
         } 
         return 0; 
     } 
  
     std::string GetSaveData() 
     { 
         return strInstData; 
     } 
  
     uint32 GetData(uint32 uiType) 
     { 
         switch(uiType) 
         { 
             case DATA_CRUSADER_ANNOUNCER: 
                 return m_uiAnnounceGUID; 
             case NPC_JAERON: 
                 return m_uiJaronGUID; 
             case NPC_ARELOS: 
                 return m_uiArelosGUID; 
             case TYPE_NORTHREND_BEAST: 
             case TYPE_ICE: 
             case TYPE_ANGELS: 
             case TYPE_JARAXXUS: 
             case TYPE_ANUBARAK: 
                 return m_auiEncounter[uiType]; 
         } 
           return 0; 
     } 
  
     void Load(const char *chrIn) 
     { 
         if (!chrIn) 
         { 
             OUT_LOAD_INST_DATA_FAIL; 
             return; 
         } 
  
         OUT_LOAD_INST_DATA(chrIn); 
  
         std::istringstream loadStream(chrIn); 
         loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3] >> m_auiEncounter[4]; 
  
         for (uint8 i = 0; i < MAX_ENCOUNTER; ++i) 
             if (m_auiEncounter[i] == IN_PROGRESS) 
                 m_auiEncounter[i] = NOT_STARTED; 
  
         OUT_LOAD_INST_DATA_COMPLETE; 
     } 
 }; 
  
 InstanceData* GetInstanceData_instance_trial_of_the_crusader(Map* pMap) 
 { 
     return new instance_trial_of_the_crusader(pMap); 
 } 
  
 void AddSC_instance_trial_of_the_crusader() 
 { 
     Script *newscript; 
     newscript = new Script; 
     newscript->Name = "instance_trial_of_the_crusader"; 
     newscript->GetInstanceData = &GetInstanceData_instance_trial_of_the_crusader; 
     newscript->RegisterSelf(); 
 }
