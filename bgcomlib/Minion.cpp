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
            set(KeywordUtil::fromString(keyword_json));
        }
    }
    if (minion_json.contains("effects")) {
        for (const auto& effect_json : minion_json["effects"]) {
            Effect effect(effect_json);
            _effects.insert({effect.trigger(), effect});
        }
    }

    _id = minion_json["dbfId"];
    _is_golden = minion_json.contains("normalDbfId");
    _alt_id = (_is_golden) ? minion_json["normalDbfId"] : minion_json["goldenDbfId"];
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
    if (_props.any()) {
        oss << " ";
    }
    if (has(Keyword::DIVINE_SHIELD)) {
        oss << "[S]";
    }
    if (has(Keyword::TAUNT)) {
        oss << "[T]";
    }
    if (has(Keyword::REBORN)) {
        oss << "[R]";
    }
    if (has(Keyword::DEATHRATTLE)) {
        oss << "[D]";
    }
    return oss.str();
}
