#ifndef MINIONDB_H
#define MINIONDB_H

#include <unordered_map>

#include "Enchantment.h"
#include "Minion.h"

class CardDb {
public:
    CardDb();
    Minion get_minion(int id);
    Enchantment get_enchantment(int id);
private:
    std::unordered_map<int, Minion> minions;
    std::unordered_map<int, Enchantment> enchantments;
};

#endif //MINIONDB_H
