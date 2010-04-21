/* Copyright (C) 2009 - 2010 Hellscream Network <http://www.hellscreamwow.com/>
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
SDName: custom_items
SD%Complete: 100
SDComment: Containts different custom Items. Check the code to know what they do. (Don't forget to apply SQL files!)
SDCategory: Item
SDAuthor: Darkiss @ Hellscream Network <http://www.hellscreamwow.com/>
EndScriptData */

#include "precompiled.h"
#include "../../../../shared/Config/Config.h"
#include "../../hn_config.h"

// Talent Points Giving Item
bool ItemUse_custom_item_talentstone(Player* pPlayer, Item* pItem, const SpellCastTargets &pTargets)
{
    Config HNConfig;
	if(!HNConfig.SetSource(_HELLSCREAM_CONFIG,true))
		error_log("HN: Unable to open configuration file");

    if ((pPlayer->isInCombat()) || (pPlayer->isInFlight()) || (pPlayer->isDead()))
    {
        pPlayer->SendEquipError(EQUIP_ERR_NOT_IN_COMBAT, pItem, NULL);
        return false;
    }
	else
    {
	pPlayer->RemoveSpellsCausingAura(SPELL_AURA_MOUNTED);
    pPlayer->SetFreeTalentPoints(pPlayer->GetFreeTalentPoints()+(HNConfig.GetFloatDefault("Item_TalentPointStoneAddTalents",0)));
    pPlayer->SendTalentsInfoData(false);
    return true;
    }
}

// Cooldown Removing Item
bool ItemUse_custom_item_cooldownremover(Player* pPlayer, Item* pItem, const SpellCastTargets &pTargets)
{
    Config HNConfig;
	if(!HNConfig.SetSource(_HELLSCREAM_CONFIG,true))
		error_log("HN: Unable to open configuration file");
		
    bool CooldownRemoverInfinte = HNConfig.GetBoolDefault("Item_CooldownRemoverInfinte", true);

    if(CooldownRemoverInfinte)
    {
        pPlayer->RemoveAllSpellCooldown();
        return false;
    }
    else
    {
        pPlayer->RemoveAllSpellCooldown();
        return true;
    }
}

// Invicible Mount Teaching Item
bool ItemUse_custom_item_invinciblemount(Player* pPlayer, Item* pItem, const SpellCastTargets &pTargets)
{
        if ((pPlayer->isInCombat()) || (pPlayer->isInFlight()) || (pPlayer->isDead()))
        {
			pPlayer->SendEquipError(EQUIP_ERR_NOT_IN_COMBAT, pItem, NULL);
			return false;
        }
		pPlayer->RemoveSpellsCausingAura(SPELL_AURA_MOUNTED);
        {
            pPlayer->learnSpell(72284, false);
            return true;
        }  
}

// Level Giving Item
bool ItemUse_custom_item_levelplayer(Player* pPlayer, Item* pItem, const SpellCastTargets &pTargets)
{
    Config HNConfig;
	if(!HNConfig.SetSource(_HELLSCREAM_CONFIG,true))
		error_log("HN: Unable to open configuration file");

    if ((pPlayer->isInCombat()) || (pPlayer->isInFlight()) || (pPlayer->isDead()))
    {
        pPlayer->SendEquipError(EQUIP_ERR_NOT_IN_COMBAT, pItem, NULL);
        return false;
    }

    if((pPlayer->getLevel()) < (HNConfig.GetFloatDefault("LevelUpItemSafeLevel",0)))
    {
	    pPlayer->RemoveSpellsCausingAura(SPELL_AURA_MOUNTED);
        pPlayer->GiveLevel(+(HNConfig.GetFloatDefault("LevelUpItemAddLevel",0)));
		return true;
    }
    else
    {
        pPlayer->SendEquipError(EQUIP_ERR_NOT_IN_COMBAT, pItem, NULL);
        return false;
    }
}

// Metamorphosis Teaching Item
bool ItemUse_custom_item_metamorphosisspell(Player* pPlayer, Item* pItem, const SpellCastTargets &pTargets)
{
    if ((pPlayer->isInCombat()) || (pPlayer->isInFlight()) || (pPlayer->isDead()))
    {
        pPlayer->SendEquipError(EQUIP_ERR_NOT_IN_COMBAT, pItem, NULL);
        return false;
    }
	else
	{
        pPlayer->RemoveSpellsCausingAura(SPELL_AURA_MOUNTED);
        pPlayer->learnSpell(59672, false);
        return true;
	}
}

// Banker NPC Summoning Item
bool ItemUse_custom_item_summonbanker(Player* pPlayer, Item* pItem, const SpellCastTargets &pTargets)
{
    Config HNConfig;
	if(!HNConfig.SetSource(_HELLSCREAM_CONFIG,true))
		error_log("HN: Unable to open configuration file");

    if ((pPlayer->isInCombat()) || (pPlayer->isInFlight()) || (pPlayer->isDead()))
    {
        pPlayer->SendEquipError(EQUIP_ERR_NOT_IN_COMBAT, pItem, NULL);
        return false;
    }
	else
    {
		pPlayer->RemoveSpellsCausingAura(SPELL_AURA_MOUNTED);
        pPlayer->SummonCreature((HNConfig.GetFloatDefault("Item_SummonBankerEntryID",0)),pPlayer->GetPositionX(), pPlayer->GetPositionY()+5, pPlayer->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,60000);
    return true;
    }
}

// Tele NPC Summoning Item
bool ItemUse_custom_item_summontelenpc(Player* pPlayer, Item* pItem, const SpellCastTargets &pTargets)
{
    Config HNConfig;
	if(!HNConfig.SetSource(_HELLSCREAM_CONFIG,true))
		error_log("HN: Unable to open configuration file");

    if ((pPlayer->isInCombat()) || (pPlayer->isInFlight()) || (pPlayer->isDead()))
    {
        pPlayer->SendEquipError(EQUIP_ERR_NOT_IN_COMBAT, pItem, NULL);
        return false;
    }
	else
    {
		pPlayer->RemoveSpellsCausingAura(SPELL_AURA_MOUNTED);
        pPlayer->SummonCreature((HNConfig.GetFloatDefault("Item_SummonTeleportMasterEntryID",0)), pPlayer->GetPositionX() ,pPlayer->GetPositionY()+5, pPlayer->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,60000);
        return true;
    }
}

void AddSC_custom_items()
{
    Script *newscript;

    newscript                   = new Script;
    newscript->Name             = "custom_item_talentstone";
    newscript->pItemUse         = &ItemUse_custom_item_talentstone;
    newscript->RegisterSelf();

    newscript                   = new Script;
    newscript->Name             = "custom_item_cooldownremover";
    newscript->pItemUse         = &ItemUse_custom_item_cooldownremover;
    newscript->RegisterSelf();

    newscript                   = new Script;
    newscript->Name             = "custom_item_invinciblemount";
    newscript->pItemUse         = &ItemUse_custom_item_invinciblemount;
    newscript->RegisterSelf();

    newscript                   = new Script;
    newscript->Name             = "custom_item_levelplayer";
    newscript->pItemUse         = &ItemUse_custom_item_levelplayer;
    newscript->RegisterSelf();

    newscript                   = new Script;
    newscript->Name             = "custom_item_metamorphosisspell";
    newscript->pItemUse         = &ItemUse_custom_item_metamorphosisspell;
    newscript->RegisterSelf();

    newscript                   = new Script;
    newscript->Name             = "custom_item_summonbanker";
    newscript->pItemUse         = &ItemUse_custom_item_summonbanker;
    newscript->RegisterSelf();

    newscript                   = new Script;
    newscript->Name             = "custom_item_summontelenpc";
    newscript->pItemUse         = &ItemUse_custom_item_summontelenpc;
    newscript->RegisterSelf();
}