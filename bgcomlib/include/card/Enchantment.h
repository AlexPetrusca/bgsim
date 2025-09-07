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

    [[nodiscard]] int id() const;

    [[nodiscard]] Target target() const;

    [[nodiscard]] const BitVector<Keyword>& props() const;

    [[nodiscard]] int attack() const;

    void set_attack(int attack);

    [[nodiscard]] int health() const;

    void set_health(int health);

private:
    int _id{};
    Target _target{};
    BitVector<Keyword> _props;
    int _attack{};
    int _health{};
};

#endif //ENCHANTMENT_H
