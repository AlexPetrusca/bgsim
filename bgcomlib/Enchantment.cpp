#include "include/card/Enchantment.h"

Enchantment::Enchantment() = default;

Enchantment::Enchantment(const json& enchant_json) {
    _id = enchant_json["dbfId"];
    _target = TargetUtil::fromString(enchant_json["target"]);
    if (enchant_json.contains("mechanics")) {
        for (const auto& keyword_json : enchant_json["mechanics"]) {
            _props.set(KeywordUtil::fromString(keyword_json));
        }
    }
    if (enchant_json.contains("attack")) {
        if (enchant_json.is_string()) {

        }
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
