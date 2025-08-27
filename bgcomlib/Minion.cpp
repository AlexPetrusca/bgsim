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
            Keyword keyword = KeywordUtil::fromString(keyword_json);
            switch (keyword) {
                case Keyword::DIVINE_SHIELD:
                    _is_divine_shield = true;
                    break;
                case Keyword::TAUNT:
                    _is_taunt = true;
                    break;
                case Keyword::REBORN:
                    _is_reborn = true;
                    break;
                case Keyword::DEATHRATTLE:
                    _is_deathrattle = true;
                    break;
                default:
                    break;
            }
            _keywords.insert(keyword);
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
    // todo: replace these with a bitvector
    if (_is_divine_shield || _is_taunt || _is_reborn || _is_deathrattle) {
        oss << " ";
    }
    if (_is_divine_shield) {
        oss << "[S]";
    }
    if (_is_taunt) {
        oss << "[T]";
    }
    if (_is_reborn) {
        oss << "[R]";
    }
    if (_is_deathrattle) {
        oss << "[D]";
    }
    return oss.str();
}
