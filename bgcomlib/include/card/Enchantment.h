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

private:
    int _id{};
    Target _target{};
    BitVector<Keyword> _props;
};

#endif //ENCHANTMENT_H
