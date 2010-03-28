/* Copyright (C) 2009 - 2010 by /dev/rsa for ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_CRUSADER_H
#define DEF_CRUSADER_H
#include "sc_boss_spell_worker.h"

enum
{
    TYPE_STAGE                  = 0,
    TYPE_BEASTS                 = 1,
    TYPE_JARAXXUS               = 2,
    TYPE_CRUSADERS              = 3,
    TYPE_VALKIRIES              = 4,
    TYPE_LICH_KING              = 5,
    TYPE_ANUBARAK               = 6,
    TYPE_COUNTER                = 7,
    TYPE_EVENT                  = 8,
    MAX_ENCOUNTERS              = 9,

    NPC_BARRENT                 = 34816,
    NPC_TIRION                  = 34996,
    NPC_FIZZLEBANG              = 35458,
    NPC_GARROSH                 = 34995,
    NPC_RINN                    = 34990,
    NPC_LICH_KING_0             = 16980,
    NPC_LICH_KING_1             = 35877,

    NPC_THRALL                  = 34994,
    NPC_PROUDMOORE              = 34992,

    NPC_ICEHOWL                 = 34797,
    NPC_GORMOK                  = 34796,
    NPC_DREADSCALE              = 34799,
    NPC_ACIDMAW                 = 35144,

    NPC_JARAXXUS                = 34780,

    NPC_CRUSADER_1_1            = 34460,
    NPC_CRUSADER_1_2            = 34463,
    NPC_CRUSADER_1_3            = 34461,
    NPC_CRUSADER_1_4            = 34471,
    NPC_CRUSADER_1_5            = 34475,
    NPC_CRUSADER_1_6            = 34472,
    NPC_CRUSADER_1_7            = 34467,
    NPC_CRUSADER_1_8            = 34468,
    NPC_CRUSADER_1_9            = 34473,
    NPC_CRUSADER_1_10           = 34474,

    NPC_CRUSADER_2_1            = 34453,
    NPC_CRUSADER_2_2            = 34455,
    NPC_CRUSADER_2_3            = 34458,
    NPC_CRUSADER_2_4            = 34454,
    NPC_CRUSADER_2_5            = 34451,
    NPC_CRUSADER_2_6            = 34456,
    NPC_CRUSADER_2_7            = 34441,
    NPC_CRUSADER_2_8            = 34449,
    NPC_CRUSADER_2_9            = 34448,
    NPC_CRUSADER_2_10           = 34450,

    NPC_CRUSADER_0_1            = 35465,
    NPC_CRUSADER_0_2            = 35610,

    NPC_LIGHTBANE               = 34497,
    NPC_DARKBANE                = 34496,

    NPC_ANUBARAK                = 34564,

    GO_CRUSADERS_CACHE_10       = 195631,
    GO_CRUSADERS_CACHE_25       = 195632,
    GO_CRUSADERS_CACHE_10_H     = 195633,
    GO_CRUSADERS_CACHE_25_H     = 195635,

    GO_TRIBUTE_CHEST_10H_25     = 195665,
    GO_TRIBUTE_CHEST_10H_45     = 195666,
    GO_TRIBUTE_CHEST_10H_50     = 195667,
    GO_TRIBUTE_CHEST_10H_99     = 195668,

    GO_TRIBUTE_CHEST_25H_25     = 195669,
    GO_TRIBUTE_CHEST_25H_45     = 195670,
    GO_TRIBUTE_CHEST_25H_50     = 195671,
    GO_TRIBUTE_CHEST_25H_99     = 195672,

    GO_ARGENT_COLISEUM_DOOR     = 195589,
    GO_ARGENT_COLISEUM_FLOOR    = 195527, //20943

    TYPE_DIFFICULTY             = 101,
    TYPE_EVENT_TIMER            = 102,
    TYPE_EVENT_NPC              = 103,
    TYPE_NORTHREND_BEASTS       = 110,

    DATA_DAMAGE_EYDIS           = 201,
    DATA_DAMAGE_FJOLA           = 202,
    DATA_CASTING_EYDIS          = 203,
    DATA_CASTING_FJOLA          = 204,

    DESPAWN_TIME                = 300000

};

static Locations SpawnLoc[]=
{
    {559.257996f, 90.266197f,  395.122986f}, // 0 Barrent
    {563.672974f, 139.571f,    393.837006f}, // 1 Center
    {563.833008f, 187.244995f, 394.5f     }, // 2 Backdoor
    {583.336182f, 181.737717f, 395.145956f}, // 3 
    {545.914185f, 180.859543f, 395.147909f}, // 4 
    {599.042358f, 179.260727f, 395.137146f}, // 5
    {532.248901f, 180.331955f, 395.132658f}, // 6
    {586.342285f, 181.515823f, 395.135956f}, // 7 
    {539.239929f, 181.825577f, 395.137909f}, // 8 
    {599.042358f, 179.260727f, 395.137146f}, // 9
    {588.720581f, 183.015472f, 395.132658f}, // 10
    {551.534119f, 184.214478f, 395.139526f}, // 11
    {576.231201f, 183.941071f, 395.139526f}, // 12
    {552.811610f, 171.985779f, 394.671661f}, // 13
    {575.641724f, 170.351501f, 394.659943f}, // 14
    {516.633972f, 159.391708f, 395.137543f}, // 15
    {612.816956f, 161.477921f, 395.136810f}, // 16
    {563.549f,    152.474f,    394.393f   }, // 17 - Lich king start
    {563.547f,    141.613f,    393.908f   }, // 18 - Lich king end
    {787.932556f, 133.28978f,  142.612152f}, // 19 - Anub'arak start location
    {618.157898f, 132.640869f, 139.559769f}, // 20 - Anub'arak move point location
    {508.104767f, 138.247345f, 395.128052f}, // 21 - Fizzlebang start location
    {586.060242f, 117.514809f, 394.314026f}, // 22 - Dark essence 1
    {541.602112f, 161.879837f, 394.587952f}, // 23 - Dark essence 2
    {541.021118f, 117.262932f, 395.314819f}, // 24 - Light essence 1
    {586.200562f, 162.145523f, 394.626129f}, // 25 - Light essence 2
    {567.967224f, 184.552048f, 395.140259f}, // 26
    {558.281250f, 184.880936f, 395.140259f}  // 27
};

enum uiWorldStates
{
    UPDATE_STATE_UI_SHOW            = 4390,
    UPDATE_STATE_UI_COUNT           = 4389,
};

enum NorthrendBeasts
{
    GORMOK_IN_PROGRESS              = 1000,
    GORMOK_DONE                     = 1001,
    DREADSCALE_SUBMERGED            = 2000,
    ACIDMAW_SUBMERGED               = 2001,
    WORMS_IN_PROGRESS               = 2002,
    ICEHOWL_IN_PROGRESS             = 1000,
    ICEHOWL_DONE                    = 1001,
};

#endif
