#ifndef ENCHANTMENT_H
#define ENCHANTMENT_H

#include <nlohmann/json.hpp>

#include "Keyword.h"
#include "Race.h"
#include "Target.h"
#include "util/BitVector.h"

using json = nlohmann::json;

class Enchantment {
public:
    Enchantment();

    explicit Enchantment(const json& enchant_json);

    [[nodiscard]] int id() const;

    [[nodiscard]] Target target() const;

    [[nodiscard]] const BitVector<Keyword>& props() const;

    [[nodiscard]] const BitVector<Race>& races() const;

    [[nodiscard]] const BitVector<Keyword>& constraints() const;

    [[nodiscard]] int attack() const;

    void set_attack(int attack);

    [[nodiscard]] int health() const;

    void set_health(int health);

    [[nodiscard]] bool has(Keyword keyword) const;

    [[nodiscard]] bool is(Race race) const;

private:
    int _id{};
    Target _target{};
    BitVector<Keyword> _props;
    BitVector<Race> _races;
    BitVector<Keyword> _constraints;
    int _attack{};
    int _health{};
};

#endif //ENCHANTMENT_H
