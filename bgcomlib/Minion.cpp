#include "include/card/Minion.h"

#include <sstream>
#include <utility>

Minion::Minion() = default;

Minion::Minion(const json& minion_json) {
    _name = minion_json["name"];
    _tier = minion_json["tier"];
    _attack = minion_json["attack"];
    _health = minion_json["health"];
    if (minion_json.contains("mechanics")) {
        for (const auto& keyword_json : minion_json["mechanics"]) {
            Keyword keyword = KeywordUtil::fromString(keyword_json.get<std::string>());
            _keywords.insert(keyword);
        }
    }
    if (minion_json.contains("effects")) {
        for (const auto& effect_json : minion_json["effects"]) {
            Effect effect(effect_json);
            _effects.insert({effect.trigger(), effect});
        }
    }
}

Minion::Minion(std::string name, int32_t tier, int32_t attack, int32_t health) {
    this->_name = std::move(name);
    this->_tier = tier;
    this->_attack = attack;
    this->_health = health;
}

[[nodiscard]] std::string Minion::to_string() {
    std::ostringstream oss;
    oss << "\"" << _name << "\" (" << _attack << "/" << _health << ")";
    return oss.str();
}
