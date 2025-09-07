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

std::string Minion::name() const {
    return _name;
}

void Minion::set_name(const std::string& name) {
    _name = name;
}

int Minion::tier() const {
    return _tier;
}

void Minion::set_tier(const int tier) {
    _tier = tier;
}

int Minion::attack() const {
    return _attack;
}

void Minion::set_attack(const int attack) {
    _attack = attack;
}

int Minion::health() const {
    return _health;
}

void Minion::set_health(const int health) {
    _health = health;
}

int Minion::delta_attack(const int delta) {
    _attack += delta;
    return _attack;
}

int Minion::delta_health(const int delta) {
    _health += delta;
    return _health;
}

int Minion::deal_damage(const int damage) {
    return delta_health(-damage);
}

const Effect& Minion::get_effect(const Keyword keyword) const {
    return _effects.at(keyword);
}

int Minion::id() const {
    return _id;
}

int Minion::alt_id() const {
    return _alt_id;
}

bool Minion::is_golden() const {
    return _is_golden;
}

bool Minion::is_zombie() const {
    return _is_zombie;
}

void Minion::set_zombie(const bool is_zombie) {
    _is_zombie = is_zombie;
}

bool Minion::is_poisoned() const {
    return _is_poisoned;
}

void Minion::set_poisoned(const bool is_poisoned) {
    _is_poisoned = is_poisoned;
}

BitVector<Keyword>& Minion::props() {
    return _props;
}

bool Minion::has(const Keyword keyword) const {
    return _props.has(keyword);
}

void Minion::set(const Keyword keyword) {
    return _props.set(keyword);
}

void Minion::clear(const Keyword keyword) {
    return _props.clear(keyword);
}

void Minion::toggle(const Keyword keyword) {
    if (_props.has(keyword)) {
        _props.clear(keyword);
    } else {
        _props.set(keyword);
    }
}

[[nodiscard]] std::string Minion::to_string() {
    std::ostringstream oss;
    oss << "\"" << _name << "\" (" << _attack << "/" << _health << ")";
    if (
        has(Keyword::DIVINE_SHIELD) || has(Keyword::TAUNT) ||
        has(Keyword::REBORN) || has(Keyword::DEATHRATTLE) ||
        has(Keyword::POISONOUS) || has(Keyword::VENOMOUS) ||
        has(Keyword::WINDFURY) || has(Keyword::MEGA_WINDFURY)
    ) {
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
    if (has(Keyword::WINDFURY)) {
        oss << "[W]";
    }
    if (has(Keyword::MEGA_WINDFURY)) {
        oss << "[MW]";
    }
    if (has(Keyword::POISONOUS)) {
        oss << "[P]";
    }
    if (has(Keyword::VENOMOUS)) {
        oss << "[V]";
    }
    return oss.str();
}
