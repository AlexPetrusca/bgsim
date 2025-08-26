#ifndef EFFECT_H
#define EFFECT_H

#include <nlohmann/json.hpp>

#include "Keyword.h"

using json = nlohmann::json;

class Effect {
public:
    enum class Type {
        SUMMON,
        GEN_CARD,
        ENCHANT
    };

    class TypeUtil {
    public:
        inline static std::unordered_map<std::string, Type> type_map {
                {"SUMMON", Type::SUMMON},
                {"GEN_CARD", Type::GEN_CARD},
                {"ENCHANT", Type::ENCHANT}
        };

        static Type fromString(const std::string& str) {
            return type_map.find(str)->second;
        }
    };

    explicit Effect(const json& json);

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
