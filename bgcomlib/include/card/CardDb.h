#ifndef MINIONDB_H
#define MINIONDB_H

#include <unordered_map>

#include "Enchantment.h"
#include "Minion.h"

class CardDb {
public:
    CardDb();

    enum class Id {
        // minions
        ALLEYCAT = 96758,
        ALLEYCAT_G = 60053,
        SEWER_RAT = 70790,
        SEWER_RAT_G = 70801,
        HOUNDMASTER = 96778,
        HOUNDMASTER_G = 59499,
        IMP_GANG_BOSS = 115949,
        IMP_GANG_BOSS_G = 58372,
        SECURITY_ROVER = 48100,
        SECURITY_ROVER_G = 58392,
        HARMLESS_BONEHEAD = 104551,
        HARMLESS_BONEHEAD_G = 104554,
        PSYCH_O_TRON = 105886,
        PSYCH_O_TRON_G = 105887,
        COLOSSUS_OF_THE_SUN = 98867,
        COLOSSUS_OF_THE_SUN_G = 98868,
        CRACKLING_CYCLONE = 64054,
        CRACKLING_CYCLONE_G = 64055,
        FOE_REAPER_4000 = 96787,
        FOE_REAPER_4000_G = 63776,
        SATED_THRESHADON = 41138,
        SATED_THRESHADON_G = 58413,
        VOIDLORD = 96797,
        VOIDLORD_G = 58427,
        VOID_EARL = 122607,
        VOID_EARL_G = 122608,
        VOIDWALKER = 96796,
        VOIDWALKER_G = 57299,
        SAVANNAH_HIGHMANE = 96793,
        SAVANNAH_HIGHMANE_G = 58409,
        DEADLY_SPORE = 65031,
        DEADLY_SPORE_G = 65122,
        TIDE_ORACLE_MORGL = 103047, // todo: not fully implemented
        TIDE_ORACLE_MORGL_G = 103064, // todo: not fully implemented
        SELFLESS_HERO = 96769,
        SELFLESS_HERO_G = 58143,
        SPAWN_OF_NZOTH = 96772,
        SPAWN_OF_NZOTH_G = 58168,
        FIENDISH_SERVANT = 56112,
        FIENDISH_SERVANT_G = 60671,
        IMPULSIVE_TRICKSTER = 72059,
        IMPULSIVE_TRICKSTER_G = 72922,
        RYLAK_METALHEAD = 98829,
        RYLAK_METALHEAD_G = 98830,
        KINDLY_GRANDMOTHER = 108851,
        KINDLY_GRANDMOTHER_G = 57340,
        SHIELDED_MINIBOT = 2026,
        SHIELDED_MINIBOT_G = 2027,
        ANNOY_O_TRON = 103652,
        ANNOY_O_TRON_G = 103653,
        SCAVENGING_HYENA = 96762,
        SCAVENGING_HYENA_G = 58395,
        DIRE_WOLF_ALPHA = 985,
        DIRE_WOLF_ALPHA_G = 60025,
        MURLOC_WARLEADER = 96768,
        MURLOC_WARLEADER_G = 57403,
        IMPRISONER = 59937,
        IMPRISONER_G = 60676,
        MECHAROO = 48886,
        MECHAROO_G = 57336,
        MICRO_MACHINE = 60055,
        MICRO_MACHINE_G = 60056,
        MURLOC_TIDECALLER = 475,
        MURLOC_TIDECALLER_G = 58138,
        MURLOC_TIDEHUNTER = 976,
        MURLOC_TIDEHUNTER_G = 57338,
        RIGHTEOUS_PROTECTOR = 42467,
        RIGHTEOUS_PROTECTOR_G = 63774,
        ROCKPOOL_HUNTER = 96761,
        ROCKPOOL_HUNTER_G = 59485,
        VULGAR_HOMUNCULUS = 43121,
        VULGAR_HOMUNCULUS_G = 63778,
        WRATH_WEAVER = 59670,
        WRATH_WEAVER_G = 59679,
        COLDLIGHT_SEER = 96773,
        COLDLIGHT_SEER_G = 59491,
        HARVEST_GOLEM = 96763,
        HARVEST_GOLEM_G = 57401,
        KABOOM_BOT = 96766,
        KABOOM_BOT_G = 58369,
        METALTOOTH_LEAPER = 96767,
        METALTOOTH_LEAPER_G = 59495,
        MOUNTED_RAPTOR = 2922,
        MOUNTED_RAPTOR_G = 86541,
        NATHREZIM_OVERSEER = 59186,
        NATHREZIM_OVERSEER_G = 59487,
        OLD_MURK_EYE = 736, // todo: can we make this implementation more general?
        OLD_MURK_EYE_G = 58382, // todo: can we make this implementation more general?
        PACK_LEADER = 59940,
        PACK_LEADER_G = 59969,
        POGO_HOPPER = 60122,
        POGO_HOPPER_G = 59664,
        ZOOBOT = 39839,
        ZOOBOT_G = 59489,
        NIGHTMARE_AMALGAM = 96780,
        NIGHTMARE_AMALGAM_G = 91216,
        ANNOY_O_MODULE = 96812,
        ANNOY_O_MODULE_G = 60327,
        COBALT_GUARDIAN = 2030,
        COBALT_GUARDIAN_G = 96813,
        CROWD_FAVORITE = 2518,
        CROWD_FAVORITE_G = 58383,
        CRYSTALWEAVER = 40391,
        CRYSTALWEAVER_G = 59504,
        FLOATING_WATCHER = 105208,
        FLOATING_WATCHER_G = 60403,
        INFESTED_WOLF = 38734,
        INFESTED_WOLF_G = 58365,
        IRON_SENSEI = 97423,
        IRON_SENSEI_G = 58397,
        KHADGAR = 96779,
        KHADGAR_G = 58380,
        PHALANX_COMMANDER = 54153,
        PHALANX_COMMANDER_G = 58385,

        // tokens
        TABBYCAT_T = 96759,
        TABBYCAT_T_G = 60054,
        HALF_SHELL_T = 70791,
        HALF_SHELL_T_G = 70802,
        SKELETON_T = 99629,
        SKELETON_T_G = 99630,
        IMP_T = 96788,
        IMP_T_G = 58373,
        GUARD_BOT_T = 49278,
        GUARD_BOT_T_G = 58393,
        PRIMALFIN_T = 41523,
        PRIMALFIN_T_G = 41525,
        HYENA_T = 96795,
        HYENA_T_G = 58410,
        BIG_BAD_WOLF_T = 108852,
        BIG_BAD_WOLF_T_G = 57341,
        JO_E_BOT_T = 48885,
        JO_E_BOT_T_G = 57337,
        MURLOC_SCOUT_T = 1078,
        MURLOC_SCOUT_T_G = 57339,
        DAMAGED_GOLEM_T = 96764,
        DAMAGED_GOLEM_T_G = 57408,
        SPIDER_T = 38735,
        SPIDER_T_G = 38736,

        // enchantments
        // todo: should these be here
        GIVE_ATTACK_E = 5,
        GIVE_HEALTH_E = 6,
        POGO_HOP_E = 26,
        POGO_HOP_G_E = 27,
    };

    [[nodiscard]] Minion get_minion(int id) const;

    [[nodiscard]] Minion get_minion(Id id) const;

    [[nodiscard]] std::unordered_map<int, Minion> minions() const;

    [[nodiscard]] std::unordered_map<int, Enchantment> enchantments() const;

    [[nodiscard]] const Enchantment& get_enchantment(int id) const;

private:
    std::unordered_map<int, Minion> _minions;
    std::unordered_map<int, Enchantment> _enchantments;
};

extern CardDb db;

#endif //MINIONDB_H
