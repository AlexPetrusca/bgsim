#ifndef EFFECT_H
#define EFFECT_H

#include <nlohmann/json.hpp>

#include "Keyword.h"

using json = nlohmann::json;

class Effect {
public:
    enum class Type {
        SUMMON,
        SUMMON_SPECIAL,
        REBORN_SUMMON,
        GEN_CARD,
        ENCHANT,
        DEAL_DAMAGE_PLAYER,
        DEAL_DAMAGE_OTHER,
        TRIGGER_ADJACENT_BATTLECRY,
        TRIGGER_ADJACENT_BATTLECRIES,
    };

    class TypeUtil {
    public:
        inline static std::unordered_map<std::string, Type> type_map{
            {"SUMMON", Type::SUMMON},
            {"SUMMON_SPECIAL", Type::SUMMON_SPECIAL},
            {"REBORN_SUMMON", Type::REBORN_SUMMON},
            {"GEN_CARD", Type::GEN_CARD},
            {"ENCHANT", Type::ENCHANT},
            {"DEAL_DAMAGE_PLAYER", Type::DEAL_DAMAGE_PLAYER},
            {"DEAL_DAMAGE_OTHER", Type::DEAL_DAMAGE_OTHER},
            {"TRIGGER_ADJACENT_BATTLECRY", Type::TRIGGER_ADJACENT_BATTLECRY},
            {"TRIGGER_ADJACENT_BATTLECRIES", Type::TRIGGER_ADJACENT_BATTLECRIES}
        };

        static Type fromString(const std::string& str);
    };

    enum class Constraint {
        NONE,

        BEAST_ONLY,
        DEMON_ONLY,
        DRAGON_ONLY,
        ELEMENTAL_ONLY,
        MECHANICAL_ONLY,
        MURLOC_ONLY,
        NAGA_ONLY,
        PIRATE_ONLY,
        QUILLBOAR_ONLY,
        UNDEAD_ONLY,

        BATTLECRY_ONLY,
        DEATHRATTLE_ONLY,
    };

    class ConstraintUtil {
    public:
        inline static std::unordered_map<std::string, Constraint> constraint_map{
            { "BEAST_ONLY", Constraint::BEAST_ONLY },
            { "DEMON_ONLY", Constraint::DEMON_ONLY },
            { "DRAGON_ONLY", Constraint::DRAGON_ONLY },
            { "ELEMENTAL_ONLY", Constraint::ELEMENTAL_ONLY },
            { "MECHANICAL_ONLY", Constraint::MECHANICAL_ONLY },
            { "MURLOC_ONLY", Constraint::MURLOC_ONLY },
            { "NAGA_ONLY", Constraint::NAGA_ONLY },
            { "PIRATE_ONLY", Constraint::PIRATE_ONLY },
            { "QUILLBOAR_ONLY", Constraint::QUILLBOAR_ONLY },
            { "UNDEAD_ONLY", Constraint::UNDEAD_ONLY },

            { "BATTLECRY_ONLY", Constraint::BATTLECRY_ONLY },
            { "DEATHRATTLE_ONLY", Constraint::DEATHRATTLE_ONLY },
        };

        static Constraint fromString(const std::string& str);
    };

    enum class SpecialSummon {
        FIRST_TWO_DEAD_MECHS = 0,
        RANDOM_TIER_1 = 1,
        RANDOM_TIER_2 = 2,
        RANDOM_TIER_3 = 3,
        RANDOM_TIER_4 = 4,
        RANDOM_TIER_5 = 5,
        RANDOM_TIER_6 = 6,
        RANDOM_TIER_7 = 7,
        RANDOM_DEATHRATTLE = 8,
        RANDOM_LEGENDARY = 9,
        RAT_PACK = 10,
        RAT_PACK_GOLDEN = 11,
    };

    explicit Effect(const json& json);

    Effect(Keyword trigger, Type type, const std::vector<int>& args);

    [[nodiscard]] Keyword trigger() const;

    [[nodiscard]] Constraint constraint() const;

    [[nodiscard]] Type type() const;

    [[nodiscard]] const std::vector<int>& args() const;

private:
    Keyword _trigger;
    Constraint _constraint;
    Type _type;
    std::vector<int> _args;
};

#endif //EFFECT_H
