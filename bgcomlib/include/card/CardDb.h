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
        HARMLESS_BONEHEAD = 104551,
        HARMLESS_BONEHEAD_G = 104554,
        PSYCH_O_TRON = 105886,
        PSYCH_O_TRON_G = 105887,
        COLOSSUS_OF_THE_SUN = 98867,
        COLOSSUS_OF_THE_SUN_G = 98868,

        // tokens
        TABBYCAT_T = 96759,
        TABBYCAT_T_G = 60054,
        HALF_SHELL_T = 70791,
        HALF_SHELL_T_G = 70802,
        SKELETON_T = 99629,
        SKELETON_T_G = 99630,
        IMP_T = 96788,
        IMP_T_G = 58373,
    };

    Minion get_minion(int id);

    Minion get_minion(Id id);

    Enchantment get_enchantment(int id);

private:
    std::unordered_map<int, Minion> minions;
    std::unordered_map<int, Enchantment> enchantments;
};

extern CardDb db;

#endif //MINIONDB_H
