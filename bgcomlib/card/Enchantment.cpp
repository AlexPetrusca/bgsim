#include "../include/card/Enchantment.h"

#include <sstream>

Enchantment::Enchantment() = default;

Enchantment::Enchantment(const json& enchant_json) {
    _id = enchant_json["dbfId"];
    _name = enchant_json["name"];
    _target = TargetUtil::fromString(enchant_json["target"]);
    if (enchant_json.contains("mechanics")) {
        for (const auto& keyword_json : enchant_json["mechanics"]) {
            _props.set(KeywordUtil::fromString(keyword_json));
        }
    }
    if (enchant_json.contains("races")) {
        for (const auto& race_json : enchant_json["races"]) {
            if (race_json == "ALL") {
                _races.set(Race::ALL);
                for (const Race race : RaceUtil::races) {
                    _races.set(race);
                }
            } else {
                _races.set(RaceUtil::fromString(race_json));
            }
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

[[nodiscard]] std::string Enchantment::to_string() {
    std::ostringstream oss;
    oss << "\"" << _name << "\" ";

    if (_attack > 0 && _health > 0) {
        oss << "(+" << _attack << "/+" << _health << ")";
    } else if (_attack > 0) {
        oss << "(+" << _attack << " attack)";
    } else {
        oss << "(+" << _health << " health)";
    }

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
