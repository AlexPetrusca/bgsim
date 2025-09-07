#ifndef ENCHANTMENT_H
#define ENCHANTMENT_H

#include <nlohmann/json.hpp>

#include "Keyword.h"
#include "Target.h"

using json = nlohmann::json;

class Enchantment {
public:
    Enchantment();

    explicit Enchantment(const json& enchant_json);

    [[nodiscard]] int id() const {
        return _id;
    }

    [[nodiscard]] Target target() const {
        return _target;
    }

    [[nodiscard]] const BitVector<Keyword>& props() const {
        return _props;
    }

    [[nodiscard]] int attack() const {
        return _attack;
    }

    [[nodiscard]] int health() const {
        return _health;
    }

private:
    int _id{};
    Target _target{};
    BitVector<Keyword> _props;
    int _attack{};
    int _health{};
};

#endif //ENCHANTMENT_H
