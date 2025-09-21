#ifndef EFFECT_H
#define EFFECT_H

#include <nlohmann/json.hpp>

#include "Keyword.h"
#include "Race.h"
#include "util/BitVector.h"

using json = nlohmann::json;

class Effect {
public:
    enum class Type {
        SUMMON,
        REBORN_SUMMON,
        GEN_CARD,
        ENCHANT,
        DEAL_DAMAGE_PLAYER,
        DEAL_DAMAGE_PLAYER_AND_ENCHANT,
        DEAL_DAMAGE_OTHER,
        TRIGGER_ADJACENT_BATTLECRY,
        TRIGGER_ADJACENT_BATTLECRIES,
    };

    class TypeUtil {
    public:
        inline static std::unordered_map<std::string, Type> type_map{
            {"SUMMON", Type::SUMMON},
            {"REBORN_SUMMON", Type::REBORN_SUMMON},
            {"GEN_CARD", Type::GEN_CARD},
            {"ENCHANT", Type::ENCHANT},
            {"DEAL_DAMAGE_PLAYER", Type::DEAL_DAMAGE_PLAYER},
            {"DEAL_DAMAGE_PLAYER_AND_ENCHANT", Type::DEAL_DAMAGE_PLAYER_AND_ENCHANT},
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
        };

        static Constraint fromString(const std::string& str);

        static bool matchesRace(Constraint constraint, const BitVector<Race>& races);
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
