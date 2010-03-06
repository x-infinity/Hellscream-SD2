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
SDName: instance_forge_of_souls
SD%Complete: 35%
SDComment:
SDCategory: Forge of Souls
EndScriptData */

#include "precompiled.h"
#include "forge_of_souls.h"

struct MANGOS_DLL_DECL instance_forge_of_souls : public ScriptedInstance
{
    instance_forge_of_souls(Map* pMap) : ScriptedInstance(pMap) {Initialize();};

    uint32 m_auiEncounter[MAX_ENCOUNTER];
    std::string strInstData;

    uint64 m_uiBronjahmGUID;
    uint64 m_uiDevourerGUID;

    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

        m_uiBronjahmGUID = 0;
        m_uiDevourerGUID = 0;
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case NPC_BRONJAHM: m_uiBronjahmGUID = pCreature->GetGUID(); break;
            case NPC_DEVOURER: m_uiDevourerGUID = pCreature->GetGUID(); break;
        }
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        debug_log("SD2: Instance Forge of Souls: SetData received for type %u with data %u", uiType, uiData);

        switch(uiType)
        {
            case TYPE_BRONJAHM:
                m_auiEncounter[0] = uiData;
                break;
            case TYPE_DEVOURER:
                m_auiEncounter[1] = uiData;
                break;
            default:
                error_log("SD2: Instance Forge of Souls: SetData = %u for type %u does not exist/not implemented.", uiType, uiData);
                break;
        }

        //saving also SPECIAL for this instance
        if (uiData == DONE || uiData == SPECIAL)
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;
            saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1];

            strInstData = saveStream.str();

            SaveToDB();
            OUT_SAVE_INST_DATA_COMPLETE;
        }
    }

    uint32 GetData(uint32 uiData)
    {
        switch(uiData)
        {
            case TYPE_BRONJAHM:
                return m_auiEncounter[0];
            case TYPE_DEVOURER:
                return m_auiEncounter[1];
        }

        return 0;
    }

    const char* Save()
    {
        return strInstData.c_str();
    }

    void Load(const char* chrIn)
    {
        if (!chrIn)
        {
            OUT_LOAD_INST_DATA_FAIL;
            return;
        }

        OUT_LOAD_INST_DATA(chrIn);

        std::istringstream loadStream(chrIn);
        loadStream >> m_auiEncounter[0] >> m_auiEncounter[1];

        for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        {
            if (m_auiEncounter[i] == IN_PROGRESS)
                m_auiEncounter[i] = NOT_STARTED;
        }

        OUT_LOAD_INST_DATA_COMPLETE;
    }
};

InstanceData* GetInstanceData_instance_forge_of_souls(Map* pMap)
{
    return new instance_forge_of_souls(pMap);
}

void AddSC_instance_forge_of_souls()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "instance_forge_of_souls";
    newscript->GetInstanceData = &GetInstanceData_instance_forge_of_souls;
    newscript->RegisterSelf();
}
