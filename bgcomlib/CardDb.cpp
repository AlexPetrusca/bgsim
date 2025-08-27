#include "card/CardDb.h"

#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

CardDb::CardDb() {
    std::ifstream file(BGCOMLIB_DIR "/rsc/minions.json");
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file!");
    }

    json minions_json;
    file >> minions_json;
    for (const auto& minion_json : minions_json) {
        minions.insert({minion_json["dbfId"], Minion(minion_json)});
    }
}

Minion CardDb::get_minion(int id) {
    return minions.at(id);
}

Minion CardDb::get_minion(Id id) {
    return minions.at(static_cast<int>(id));
}

Enchantment CardDb::get_enchantment(int id) {
    return enchantments.at(id);
}

CardDb db = CardDb();
