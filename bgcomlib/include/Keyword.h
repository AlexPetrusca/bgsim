#ifndef KEYWORD_H
#define KEYWORD_H

enum class Keyword {
    // Official
    DEATHRATTLE,
    REBORN,
    BATTLECRY,
    POISONOUS,
    VENOMOUS,
    TAUNT,
    DIVINE_SHIELD,
    MAGNETIC,
    RALLY,
    OVERKILL,
    WINDFURY,

    // Custom
    CLEAVE,
    IMMUNE,
    AURA,
    START_OF_TURN,
    END_OF_TURN,
    ON_DAMAGE_SELF,
    ON_DAMAGE_PLAYER,
    ON_SUMMON,
    ON_ATTACK,
    ON_KILL,
    ON_DEATH_OTHER,

    // Card-Specific
    ZAPP_SLYWICK,
};

namespace KeywordUtil {
    inline std::map<std::string, Keyword> keyword_map = {
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

        {"CLEAVE", Keyword::CLEAVE},
        {"IMMUNE", Keyword::IMMUNE},
        {"AURA", Keyword::AURA},
        {"START_OF_TURN", Keyword::START_OF_TURN},
        {"END_OF_TURN", Keyword::END_OF_TURN},
        {"ON_DAMAGE_SELF", Keyword::ON_DAMAGE_SELF},
        {"ON_DAMAGE_PLAYER", Keyword::ON_DAMAGE_PLAYER},
        {"ON_SUMMON", Keyword::ON_SUMMON},
        {"ON_ATTACK", Keyword::ON_ATTACK},
        {"ON_KILL", Keyword::ON_KILL},
        {"ON_DEATH_OTHER", Keyword::ON_DEATH_OTHER},

        {"ZAPP_SLYWICK", Keyword::ZAPP_SLYWICK},
    };

    inline Keyword fromString(const std::string& str) {
        return keyword_map.find(str)->second;
    }
}

#endif //KEYWORD_H
