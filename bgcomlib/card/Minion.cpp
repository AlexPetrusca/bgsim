#include "../include/card/Minion.h"

#include <sstream>
#include <utility>

Minion::Minion() = default;

Minion::Minion(const json& json) {
    _name = json["name"];
    _tier = json["tier"];
    _attack = json["attack"];
    _health = json["health"];
    _max_health = json["health"];
    if (json.contains("races")) {
        for (const auto& race_json : json["races"]) {
            _races.set(RaceUtil::fromString(race_json));
        }
    }
    if (json.contains("mechanics")) {
        for (const auto& keyword_json : json["mechanics"]) {
            set(KeywordUtil::fromString(keyword_json));
        }
    }
    if (json.contains("effects")) {
        for (const auto& effect_json : json["effects"]) {
            Effect effect(effect_json);
            _effects.insert({effect.trigger(), effect});
        }
    }

    _id = json["dbfId"];
    _is_golden = json.contains("normalDbfId");
    _alt_id = (_is_golden) ? json["normalDbfId"] : json["goldenDbfId"];
}

Minion::Minion(std::string name, const int tier, const int attack, const int health) {
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

int Minion::max_health() const {
    return _max_health;
}

void Minion::set_max_health(const int max_health) {
    _max_health = max_health;
}

int Minion::aura_attack() const {
    return _aura_attack;
}

void Minion::set_aura_attack(const int aura_attack) {
    _aura_attack = aura_attack;
}

int Minion::aura_health() const {
    return _aura_health;
}

void Minion::set_aura_health(const int aura_health) {
    _aura_health = aura_health;
}

void Minion::disable_aura(const bool permanent) {
    delta_attack(-_aura_attack, true);
    delta_health(-_aura_health, true);
    if (permanent && _health <= 0) {
        _health = 1; // minions can't die from losing aura
    }
}

int Minion::delta_attack(const int delta, const bool aura) {
    if (aura) {
        _aura_attack += delta;
    }
    _attack += delta;
    return _attack;
}

int Minion::delta_health(const int delta, const bool aura) {
    if (aura) {
        _aura_health += delta;
        _max_health += delta;
    } else if (delta > 0) {
        _max_health += delta;
    }
    _health += delta;
    return _health;
}

int Minion::deal_damage(const int damage) {
    return delta_health(-damage);
}

const std::unordered_map<Keyword, Effect>& Minion::effects() const {
    return _effects;
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

BitVector<Race>& Minion::races() {
    return _races;
}

bool Minion::has(const Keyword keyword) const {
    return _props.has(keyword);
}

bool Minion::is(const Race race) const {
    return _races.has(race);
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

Minion* Minion::left_adjacent() const {
    return _left;
}

void Minion::set_left_adjacent(Minion* left) {
    _left = left;
}

Minion* Minion::right_adjacent() const {
    return _right;
}

void Minion::set_right_adjacent(Minion* right) {
    _right = right;
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
