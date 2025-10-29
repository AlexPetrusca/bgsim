#ifndef ENCHANTMENT_H
#define ENCHANTMENT_H

#include <nlohmann/json.hpp>

#include "Card.h"
#include "Effect.h"
#include "Keyword.h"
#include "Race.h"
#include "Target.h"
#include "util/BitVector.h"

using json = nlohmann::json;

class Enchantment : public Card {
public:
    Enchantment();

    explicit Enchantment(const json& enchant_json);

    [[nodiscard]] Target target() const;

    [[nodiscard]] const BitVector<Keyword>& props() const;

    [[nodiscard]] const BitVector<Race>& races() const;

    [[nodiscard]] const BitVector<Keyword>& constraints() const;

    [[nodiscard]] const std::unordered_map<Keyword, std::vector<Effect>>& effects() const;

    [[nodiscard]] const std::vector<Effect>& get_effects(Keyword keyword) const;

    [[nodiscard]] int attack() const;

    void set_attack(int attack);

    [[nodiscard]] int health() const;

    void set_health(int health);

    [[nodiscard]] bool has(Keyword keyword) const;

    [[nodiscard]] bool is(Race race) const;

    void merge(const Enchantment& other);

    [[nodiscard]] std::string to_string() override;

private:
    Target _target{};
    BitVector<Keyword> _props;
    BitVector<Race> _races;
    BitVector<Keyword> _constraints;
    std::unordered_map<Keyword, std::vector<Effect>> _effects;
    int _attack{};
    int _health{};
};

#endif //ENCHANTMENT_H
