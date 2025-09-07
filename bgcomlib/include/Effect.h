#ifndef EFFECT_H
#define EFFECT_H

#include <nlohmann/json.hpp>

#include "Keyword.h"

using json = nlohmann::json;

class Effect {
public:
    enum class Type {
        SUMMON,
        REBORN_SUMMON,
        GEN_CARD,
        ENCHANT,
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
            {"TRIGGER_ADJACENT_BATTLECRY", Type::TRIGGER_ADJACENT_BATTLECRY},
            {"TRIGGER_ADJACENT_BATTLECRIES", Type::TRIGGER_ADJACENT_BATTLECRIES}
        };

        static Type fromString(const std::string& str) {
            return type_map.find(str)->second;
        }
    };

    explicit Effect(const json& json);

    Effect(Keyword trigger, Type type, const std::vector<int>& args);

    [[nodiscard]] Keyword trigger() const {
        return _trigger;
    }

    [[nodiscard]] Type type() const {
        return _type;
    }

    [[nodiscard]] const std::vector<int>& args() const {
        return _args;
    }

private:
    Keyword _trigger;
    Type _type;
    std::vector<int> _args;
};

#endif //EFFECT_H
