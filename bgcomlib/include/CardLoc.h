#ifndef MINIONLOC_H
#define MINIONLOC_H

#include <list>
#include <unordered_set>

#include "card/Enchantment.h"
#include "card/Minion.h"

using CardLoc = std::list<std::shared_ptr<Card>>::iterator;

class CardUtil {
public:
    static Card& as_card(const CardLoc loc) {
        return *loc->get();
    }

    static Minion& as_minion(const CardLoc loc) {
        return *dynamic_cast<Minion*>(loc->get());
    }

    static Enchantment& as_enchantment(const CardLoc loc) {
        return *dynamic_cast<Enchantment*>(loc->get());
    }

    static Card& as_card(const std::shared_ptr<Card>& card) {
        return *card;
    }

    static Minion& as_minion(const std::shared_ptr<Card>& card) {
        return dynamic_cast<Minion&>(*card);
    }

    static Enchantment& as_enchantment(const std::shared_ptr<Card>& card) {
        return dynamic_cast<Enchantment&>(*card);
    }
};

using MinionLoc = std::list<Minion>::iterator;

struct MinionLocHash {
    std::size_t operator()(const std::list<Minion>::iterator& it) const noexcept {
        return std::hash<Minion*>{}(&*it); // hash the pointer to the element
    }
};

struct MinionLocEq {
    bool operator()(const std::list<Minion>::iterator& a, const std::list<Minion>::iterator& b) const noexcept {
        return &*a == &*b;
    }
};

using MinionLocSet = std::unordered_set<MinionLoc, MinionLocHash, MinionLocEq>;


#endif //MINIONLOC_H
