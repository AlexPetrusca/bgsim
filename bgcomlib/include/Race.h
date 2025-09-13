#ifndef RACE_H
#define RACE_H

#include <string>
#include <unordered_map>

enum class Race {
    NONE = 0,

    ALL = 1ull << 1,
    BEAST = 1ull << 2,
    DEMON = 1ull << 3,
    DRAGON = 1ull << 4,
    ELEMENTAL = 1ull << 5,
    MECHANICAL = 1ull << 6,
    MURLOC = 1ull << 7,
    NAGA = 1ull << 8,
    PIRATE = 1ull << 9,
    QUILLBOAR = 1ull << 10,
    UNDEAD = 1ull << 11,
};

namespace RaceUtil {
    inline std::unordered_map<std::string, Race> race_map = {
        {"ALL", Race::ALL},
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
    };

    inline Race fromString(const std::string& str) {
        return race_map.find(str)->second;
    }
}

#endif //RACE_H
