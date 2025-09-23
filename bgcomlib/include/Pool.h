#ifndef POOL_H
#define POOL_H

#include <unordered_map>

#include "card/CardDb.h"

class Pool {
public:
    Pool();

    [[nodiscard]] CardDb::Id get_random_minionid_by_tier(int tier) const;

    [[nodiscard]] const std::unordered_map<CardDb::Id, int>& get_tier(int tier) const;

    [[nodiscard]] int total_count() const;

    [[nodiscard]] int tier_count(int tier) const;

    [[nodiscard]] const std::vector<int>& tier_counts() const;

private:
    std::unordered_map<int, std::unordered_map<CardDb::Id, int>> _pool; // tier -> id -> count
    int _total_count{};
    std::vector<int> _tier_counts;

    inline static std::vector<CardDb::Id> _tier1 {
        CardDb::Id::ALLEYCAT,
        CardDb::Id::ANNOY_O_TRON,
        CardDb::Id::DIRE_WOLF_ALPHA,
        CardDb::Id::FIENDISH_SERVANT,
        CardDb::Id::IMPRISONER,
        CardDb::Id::MECHAROO,
        CardDb::Id::MICRO_MACHINE,
        CardDb::Id::MURLOC_TIDECALLER,
        CardDb::Id::MURLOC_TIDEHUNTER,
        CardDb::Id::RIGHTEOUS_PROTECTOR,
        CardDb::Id::ROCKPOOL_HUNTER,
        CardDb::Id::SCAVENGING_HYENA,
        CardDb::Id::VULGAR_HOMUNCULUS,
        CardDb::Id::WRATH_WEAVER,

        CardDb::Id::VOIDWALKER,
    };

    inline static std::vector<CardDb::Id> _tier2 {
        CardDb::Id::KINDLY_GRANDMOTHER,
        CardDb::Id::SHIELDED_MINIBOT,
        CardDb::Id::COLDLIGHT_SEER,
        CardDb::Id::HARVEST_GOLEM,
        CardDb::Id::KABOOM_BOT,
        CardDb::Id::METALTOOTH_LEAPER,
        // CardDb::Id::MOUNTED_RAPTOR,
        CardDb::Id::MURLOC_WARLEADER,
        // CardDb::Id::NATHREZIM_OVERSEER,
        // CardDb::Id::OLD_MURK_EYE,
        // CardDb::Id::PACK_LEADER,
        // CardDb::Id::POGO_HOPPER,
        CardDb::Id::SELFLESS_HERO,
        CardDb::Id::SPAWN_OF_NZOTH,
        // CardDb::Id::ZOOBOT,
    };

    inline static std::vector<CardDb::Id> _tier3 {
        CardDb::Id::IMP_GANG_BOSS,
        // CardDb::Id::ANNOY_O_MODULE,
        // CardDb::Id::COBALT_GUARDIAN,
        // CardDb::Id::CROWD_FAVORITE,
        // CardDb::Id::CRYSTALWEAVER,
        // CardDb::Id::FLOATING_WATCHER,
        CardDb::Id::HOUNDMASTER,
        // CardDb::Id::INFESTED_WOLF,
        // CardDb::Id::IRON_SENSEI,
        // CardDb::Id::KHADGAR,
        // CardDb::Id::PHALANX_COMMANDER,
        // CardDb::Id::PILOTED_SHREDDER,
        CardDb::Id::PSYCH_O_TRON,
        // CardDb::Id::RAT_PACK,
        // CardDb::Id::REPLICATING_MENACE,
        // CardDb::Id::SCREWJANK_CLUNKER,
        // CardDb::Id::SHIFTER_ZERUS,
        // CardDb::Id::SOUL_JUGGLER,
        // CardDb::Id::THE_BEAST,
        // CardDb::Id::TORTOLLAN_SHELLRAISER,

        // CardDb::Id::NIGHTMARE_AMALGAM,
    };

    inline static std::vector<CardDb::Id> _tier4 {
        // CardDb::Id::ANNIHILAN_BATTLEMASTER,
        // CardDb::Id::BOLVAR_FIREBLOOD,
        // CardDb::Id::CAVE_HYDRA,
        // CardDb::Id::DEFENDER_OF_ARGUS,
        // CardDb::Id::JUNKBOT,
        // CardDb::Id::KING_BAGURGLE,
        // CardDb::Id::MAMA_BEAR,
        // CardDb::Id::MECHANO_EGG,
        // CardDb::Id::MENAGERIE_MAGICIAN,
        // CardDb::Id::PILOTED_SKY_GOLEM,
        CardDb::Id::SAVANNAH_HIGHMANE,
        CardDb::Id::SECURITY_ROVER,
        // CardDb::Id::SIEGEBREAKER,
        // CardDb::Id::STRONGSHELL_SCAVENGER,
        // CardDb::Id::THE_BOOGEYMONSTER,
        // CardDb::Id::VIRMEN_SENSEI,
    };

    inline static std::vector<CardDb::Id> _tier5 {
        // CardDb::Id::BARON_RIVENDARE,
        // CardDb::Id::BRANN_BRONZEBEARD,
        // CardDb::Id::FESTERROOT_HULK,
        // CardDb::Id::IRONHIDE_DIREHORN,
        // CardDb::Id::KANGORS_APPRENTICE,
        // CardDb::Id::LIGHTFANG_ENFORCER,
        // CardDb::Id::MALGANIS,
        // CardDb::Id::PRIMALFIN_LOOKOUT,
        CardDb::Id::SATED_THRESHADON,
        // CardDb::Id::SNEEDS_OLD_SHREDDER,
        // CardDb::Id::TOXFIN,
        CardDb::Id::VOIDLORD,
    };

    inline static std::vector<CardDb::Id> _tier6 {
        CardDb::Id::FOE_REAPER_4000,
        // CardDb::Id::GENTLE_MEGASAUR,
        // CardDb::Id::GHASTCOILER,
        // CardDb::Id::GOLDRINN_THE_GREAT_WOLF,
        // CardDb::Id::MAEXXNA,
        // CardDb::Id::ZAPP_SLYWICK,
    };

    inline static std::vector<CardDb::Id> _tier7 {
        // todo: add the rest
        CardDb::Id::TIDE_ORACLE_MORGL,
    };

    inline static std::vector<std::vector<CardDb::Id>> _tiers {
        _tier1, _tier2, _tier3, _tier4, _tier5, _tier6, _tier7,
    };

    inline static std::vector<int> _copies_by_tier {
        15, 15, 13, 11, 9, 7, 5,
    };
};

#endif //POOL_H
