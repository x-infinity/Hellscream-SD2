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
SDName: boss_devourer_of_souls
SD%Complete: 0%
SDComment:
SDCategory: The Forge of Souls
EndScriptData */

#include "precompiled.h"
#include "forge_of_souls.h"

enum
{
    SPEEL_PHANTOM_BLAST_N = 68982,
    SPEEL_PHANTOM_BLAST_H = 70322,
    SPELL_MIRRORED_SOUL   = 69051,
    SPELL_WELL_OF_SOULS   = 68920, // visual effect?
    SPELL_WELL_OF_SOULS_N = 68863,
    SPELL_WELL_OF_SOULS_H = 70323,
    SPELL_UNLEASHED_SOULS = 68939,
    SPELL_WAILING_SOULS1  = 68912,
    SPELL_WAILING_SOULS2  = 68899,
    SPELL_WAILING_SOULS_N = 68873,
    SPELL_WAILING_SOULS_H = 70324
};

struct MANGOS_DLL_DECL boss_devourer_of_soulsAI : public ScriptedAI
{
    boss_devourer_of_soulsAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    void Reset()
    {
    }

    void Aggro(Unit* pWho)
    {
        //DoScriptText(SAY_AGGRO, m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
        /*switch(urand(0, 3))
        {
            case 0: DoScriptText(SAY_SLAY_1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY_2, m_creature); break;
            case 2: DoScriptText(SAY_SLAY_3, m_creature); break;
            case 3: DoScriptText(SAY_SLAY_4, m_creature); break;
        }*/
    }

    void JustDied(Unit* pKiller)
    {
        //DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_DEVOURER, DONE);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_devourer_of_souls(Creature* pCreature)
{
    return new boss_devourer_of_soulsAI(pCreature);
}

void AddSC_boss_devourer_of_souls()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_devourer_of_souls";
    newscript->GetAI = &GetAI_boss_devourer_of_souls;
    newscript->RegisterSelf();
}