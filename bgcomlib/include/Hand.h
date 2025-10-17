#ifndef HAND_H
#define HAND_H

#include <list>

#include "CardLoc.h"
#include "card/CardDb.h"
#include "card/Minion.h"

class Hand {
public:
    static constexpr int MAX_HAND_SIZE{10};

    Hand();

    bool add_minion(const Minion& minion);

    bool add_minion(CardDb::Id id);

    void remove_card(CardLoc loc);

    void remove_card(int idx);

    CardLoc begin();

    CardLoc end();

    std::list<std::shared_ptr<Card>>& cards();

    int size();

    bool empty();

    bool full();

private:
    std::list<std::shared_ptr<Card>> _cards;
};

#endif //HAND_H
