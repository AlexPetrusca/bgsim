#include "../include/card/Enchantment.h"

Enchantment::Enchantment() = default;

Enchantment::Enchantment(const json& enchant_json) {
    _id = enchant_json["dbfId"];
    _target = TargetUtil::fromString(enchant_json["target"]);
    if (enchant_json.contains("mechanics")) {
        for (const auto& keyword_json : enchant_json["mechanics"]) {
            _props.set(KeywordUtil::fromString(keyword_json));
        }
    }
    if (enchant_json.contains("races")) {
        for (const auto& race_json : enchant_json["races"]) {
            _races.set(RaceUtil::fromString(race_json));
        }
    }
    if (enchant_json.contains("constraints")) {
        for (const auto& keyword_json : enchant_json["constraints"]) {
            _constraints.set(KeywordUtil::fromString(keyword_json));
        }
    }
    if (enchant_json.contains("attack")) {
        _attack = enchant_json["attack"];
    }
    if (enchant_json.contains("health")) {
        _health = enchant_json["health"];
    }
}

int Enchantment::id() const {
    return _id;
}

Target Enchantment::target() const {
    return _target;
}

const BitVector<Keyword>& Enchantment::props() const {
    return _props;
}

const BitVector<Race>& Enchantment::races() const {
    return _races;
}

const BitVector<Keyword>& Enchantment::constraints() const {
    return _constraints;
}

int Enchantment::attack() const {
    return _attack;
}

void Enchantment::set_attack(const int attack) {
    _attack = attack;
}

int Enchantment::health() const {
    return _health;
}

void Enchantment::set_health(const int health) {
    _health = health;
}

bool Enchantment::has(const Keyword keyword) const {
    return _props.has(keyword);

}

bool Enchantment::is(const Race race) const {
    return _races.has(race);

}
