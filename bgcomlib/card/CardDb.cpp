#include "card/CardDb.h"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

CardDb::CardDb() {
    // todo: don't load data in constructor - do this in an init method instead
    std::ifstream minions_file(BGCOMLIB_DIR "/rsc/minions.json");
    if (!minions_file.is_open()) {
        throw std::runtime_error("Could not open file!");
    }
    json minions_json;
    minions_file >> minions_json;
    for (const auto& minion_json : minions_json) {
        _minions.insert({minion_json["dbfId"], Minion(minion_json)});
    }

    std::ifstream enchantments_file(BGCOMLIB_DIR "/rsc/enchantments.json");
    if (!enchantments_file.is_open()) {
        throw std::runtime_error("Could not open file!");
    }
    json enchantments_json;
    enchantments_file >> enchantments_json;
    for (const auto& enchant_json : enchantments_json) {
        _enchantments.insert({enchant_json["dbfId"], Enchantment(enchant_json)});
    }
}

Minion CardDb::get_minion(const int id) const {
    return _minions.at(id);
}

Minion CardDb::get_minion(Id id) const {
    return _minions.at(static_cast<int>(id));
}

std::unordered_map<int, Minion> CardDb::minions() const {
    return _minions;
}

std::unordered_map<int, Enchantment> CardDb::enchantments() const {
    return _enchantments;
}

const Enchantment& CardDb::get_enchantment(const int id) const {
    return _enchantments.at(id);
}

CardDb db = CardDb();
