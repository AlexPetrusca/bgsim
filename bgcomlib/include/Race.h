#ifndef RACE_H
#define RACE_H

#include <string>
#include <unordered_map>

enum class Race {
    BEAST = 1 << 0,
    DEMON = 1 << 1,
    DRAGON = 1 << 2,
    ELEMENTAL = 1 << 3,
    MECHANICAL = 1 << 4,
    MURLOC = 1 << 5,
    NAGA = 1 << 6,
    PIRATE = 1 << 7,
    QUILLBOAR = 1 << 8,
    UNDEAD = 1 << 9,
    ALL = 1 << 10,
};

namespace RaceUtil {
    inline std::unordered_map<std::string, Race> race_map = {
        {"BEAST", Race::BEAST},
        {"DEMON", Race::DEMON},
        {"DRAGON", Race::DRAGON},
        {"ELEMENTAL", Race::ELEMENTAL},
        {"MECHANICAL", Race::MECHANICAL},
        {"MURLOC", Race::MURLOC},
        {"NAGA", Race::NAGA},
        {"PIRATE", Race::PIRATE},
        {"QUILLBOAR", Race::QUILLBOAR},
        {"UNDEAD", Race::UNDEAD},
        {"ALL", Race::ALL},
    };

    inline std::vector races = {
        Race::BEAST,
        Race::DEMON,
        Race::DRAGON,
        Race::ELEMENTAL,
        Race::MECHANICAL,
        Race::MURLOC,
        Race::NAGA,
        Race::PIRATE,
        Race::QUILLBOAR,
        Race::UNDEAD,
    };

    inline Race fromString(const std::string& str) {
        return race_map.find(str)->second;
    }
}

#endif //RACE_H
