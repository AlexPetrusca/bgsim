#ifndef KEYWORD_H
#define KEYWORD_H

#include <string>
#include <unordered_map>
#include <cstdint>

enum class Keyword: uint64_t {
    // Official
    DEATHRATTLE = 1ull << 0,
    REBORN = 1ull << 1,
    BATTLECRY = 1ull << 2,
    POISONOUS = 1ull << 3,
    VENOMOUS = 1ull << 4,
    TAUNT = 1ull << 5,
    DIVINE_SHIELD = 1ull << 6,
    MAGNETIC = 1ull << 7,
    RALLY = 1ull << 8,
    OVERKILL = 1ull << 9,
    WINDFURY = 1ull << 10,
    MEGA_WINDFURY = 1ull << 11,
    STEALTH = 1ull << 12,
    CHOOSE_ONE = 1ull << 13,

    // Custom
    CLEAVE = 1ull << 14,
    IMMUNE = 1ull << 15,
    AURA = 1ull << 16,
    ADJACENT_AURA = 1ull << 17,
    ZAPP_SLYWICK = 1ull << 18,

    // Triggers
    ON_TURN_START = 1ull << 19,
    ON_TURN_END = 1ull << 20,
    ON_DAMAGE_SELF = 1ull << 21,
    ON_DAMAGE_OTHER = 1ull << 22,
    ON_DAMAGE_PLAYER = 1ull << 23,
    ON_ADD = 1ull << 24,
    ON_PLAY = 1ull << 25,
    ON_SUMMON = 1ull << 26,
    ON_ATTACK = 1ull << 27,
    ON_KILL = 1ull << 28,
    ON_DEATH_OTHER = 1ull << 29,
    ON_PRE_COMBAT = 1ull << 30,
    ON_POST_COMBAT = 1ull << 31,
};

namespace KeywordUtil {
    inline std::unordered_map<std::string, Keyword> keyword_map = {
        {"DEATHRATTLE", Keyword::DEATHRATTLE},
        {"REBORN", Keyword::REBORN},
        {"BATTLECRY", Keyword::BATTLECRY},
        {"POISONOUS", Keyword::POISONOUS},
        {"VENOMOUS", Keyword::VENOMOUS},
        {"TAUNT", Keyword::TAUNT},
        {"DIVINE_SHIELD", Keyword::DIVINE_SHIELD},
        {"MAGNETIC", Keyword::MAGNETIC},
        {"RALLY", Keyword::RALLY},
        {"OVERKILL", Keyword::OVERKILL},
        {"WINDFURY", Keyword::WINDFURY},
        {"MEGA_WINDFURY", Keyword::MEGA_WINDFURY},
        {"STEALTH", Keyword::STEALTH},
        {"CHOOSE_ONE", Keyword::CHOOSE_ONE},

        {"CLEAVE", Keyword::CLEAVE},
        {"IMMUNE", Keyword::IMMUNE},
        {"AURA", Keyword::AURA},
        {"ADJACENT_AURA", Keyword::ADJACENT_AURA},
        {"ZAPP_SLYWICK", Keyword::ZAPP_SLYWICK},

        {"ON_TURN_START", Keyword::ON_TURN_START},
        {"ON_TURN_END", Keyword::ON_TURN_END},
        {"ON_DAMAGE_SELF", Keyword::ON_DAMAGE_SELF},
        {"ON_DAMAGE_OTHER", Keyword::ON_DAMAGE_OTHER},
        {"ON_DAMAGE_PLAYER", Keyword::ON_DAMAGE_PLAYER},
        {"ON_ADD", Keyword::ON_ADD},
        {"ON_PLAY", Keyword::ON_PLAY},
        {"ON_SUMMON", Keyword::ON_SUMMON},
        {"ON_ATTACK", Keyword::ON_ATTACK},
        {"ON_KILL", Keyword::ON_KILL},
        {"ON_DEATH_OTHER", Keyword::ON_DEATH_OTHER},
        {"ON_PRE_COMBAT", Keyword::ON_PRE_COMBAT},
        {"ON_POST_COMBAT", Keyword::ON_POST_COMBAT},
    };

    inline Keyword fromString(const std::string& str) {
        return keyword_map.find(str)->second;
    }

    inline bool isTrigger(Keyword keyword) {
        return static_cast<uint64_t>(keyword) >= static_cast<uint64_t>(Keyword::ON_TURN_START);
    }
}

#endif //KEYWORD_H
