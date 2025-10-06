#ifndef HAND_H
#define HAND_H

#include <list>

#include "MinionLoc.h"
#include "card/CardDb.h"
#include "card/Minion.h"

class Hand {
public:
    static constexpr int MAX_HAND_SIZE{10};

    Hand();

    bool add_card(const Minion& minion);

    bool add_card(CardDb::Id id);

    void remove_card(MinionLoc loc);

    void remove_card(int index);

    MinionLoc begin();

    MinionLoc end();

    std::list<Minion>& cards();

    int size();

private:
    std::list<Minion> _cards;
};

#endif //HAND_H
