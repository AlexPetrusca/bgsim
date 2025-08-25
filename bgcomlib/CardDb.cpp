#include "card/CardDb.h"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Minion parseMinion(const json& minions_json) {
    return {
        minions_json["name"],
        minions_json["tier"],
        minions_json["attack"],
        minions_json["health"]
    };
}

CardDb::CardDb() {
    std::ifstream file(BGCOMLIB_DIR "/rsc/minions.json");
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file!");
    }

    json minions_json;
    file >> minions_json;
    for (const auto& minion_json : minions_json) {
        minions.insert({minion_json["dbfId"], parseMinion(minion_json)});
    }
}

Minion CardDb::get_minion(int id) {
    return minions.at(id);
}

Enchantment CardDb::get_enchantment(int id) {
    return enchantments.at(id);
}
