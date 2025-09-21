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
            {"DEAL_DAMAGE_OTHER", Type::DEAL_DAMAGE_OTHER},
            {"TRIGGER_ADJACENT_BATTLECRY", Type::TRIGGER_ADJACENT_BATTLECRY},
            {"TRIGGER_ADJACENT_BATTLECRIES", Type::TRIGGER_ADJACENT_BATTLECRIES}
        };

        static Type fromString(const std::string& str) {
            return type_map.find(str)->second;
        }
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

        static Constraint fromString(const std::string& str) {
            const auto pair = constraint_map.find(str);
            if (pair != constraint_map.end()) {
                return pair->second;
            } else {
                return Constraint::NONE;
            }
        }

        static bool matchesRace(const Constraint constraint, const BitVector<Race>& races) {
            // todo: can we us this with some clever logic instead?
            //  - Maybe you can just use bit manipulation
            switch (constraint) {
                case Constraint::NONE:
                    return true;
                case Constraint::BEAST_ONLY:
                    return races.has(Race::BEAST);
                case Constraint::DEMON_ONLY:
                    return races.has(Race::DEMON);
                case Constraint::DRAGON_ONLY:
                    return races.has(Race::DRAGON);
                case Constraint::ELEMENTAL_ONLY:
                    return races.has(Race::ELEMENTAL);
                case Constraint::MECHANICAL_ONLY:
                    return races.has(Race::MECHANICAL);
                case Constraint::MURLOC_ONLY:
                    return races.has(Race::MURLOC);
                case Constraint::NAGA_ONLY:
                    return races.has(Race::NAGA);
                case Constraint::PIRATE_ONLY:
                    return races.has(Race::PIRATE);
                case Constraint::QUILLBOAR_ONLY:
                    return races.has(Race::QUILLBOAR);
                case Constraint::UNDEAD_ONLY:
                    return races.has(Race::UNDEAD);
            }
            return false;
        }
    };

    explicit Effect(const json& json);

    Effect(Keyword trigger, Type type, const std::vector<int>& args);

    [[nodiscard]] Keyword trigger() const {
        return _trigger;
    }

    [[nodiscard]] Constraint constraint() const {
        return _constraint;
    }

    [[nodiscard]] Type type() const {
        return _type;
    }

    [[nodiscard]] const std::vector<int>& args() const {
        return _args;
    }

private:
    Keyword _trigger;
    Constraint _constraint;
    Type _type;
    std::vector<int> _args;
};

#endif //EFFECT_H
