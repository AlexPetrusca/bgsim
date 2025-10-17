#ifndef BGSIMROOT_DISCOVERIES_H
#define BGSIMROOT_DISCOVERIES_H

#include <list>

#include "CardLoc.h"
#include "Pool.h"
#include "card/CardDb.h"
#include "card/Minion.h"

// todo: this should handle all accesses to the Pool
class Discovers {
public:
    Discovers();

    void discover(Effect::Discover type, int tier, const Minion* source);

    void adapt();

    Minion select(int idx);

    void add_card(const Minion& minion);

    void add_card(CardDb::Id id);

    void remove_card(MinionLoc loc);

    void remove_card(int idx);

    MinionLoc begin();

    MinionLoc end();

    std::list<Minion>& cards();

    int size();

    void bind_pool(Pool* pool);

private:
    std::list<Minion> _cards;

    Pool* _pool{}; // todo: should pool just be a global?
};

#endif //BGSIMROOT_DISCOVERIES_H