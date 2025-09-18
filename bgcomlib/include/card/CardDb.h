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

        // enchantments
        GIVE_ATTACK_E = 5,
        GIVE_HEALTH_E = 6,
    };

    [[nodiscard]] Minion get_minion(int id) const;

    [[nodiscard]] Minion get_minion(Id id) const;

    [[nodiscard]] const Enchantment& get_enchantment(int id) const;

private:
    std::unordered_map<int, Minion> minions;
    std::unordered_map<int, Enchantment> enchantments;
};

extern CardDb db;

#endif //MINIONDB_H
