#include "include/Hand.h"

Hand::Hand() = default;

bool Hand::add_minion(const Minion& minion) {
    if (full()) return false;

    _cards.push_back(std::make_shared<Minion>(std::move(minion)));
    return true;
}

bool Hand::add_minion(const CardDb::Id id) {
    return add_minion(db.get_minion(id));
}

void Hand::remove_card(const CardLoc loc) {
    _cards.erase(loc);
}

void Hand::remove_card(const int idx) {
    assert(idx >= _cards.size() && "Out of bounds!");

    CardLoc loc = _cards.begin();
    for (int i = 0; i < idx; i++) {
        ++loc;
    }
    remove_card(loc);
}

CardLoc Hand::begin() {
    return _cards.begin();
}

CardLoc Hand::end() {
    return _cards.end();
}

std::list<std::shared_ptr<Card>>& Hand::cards() {
    return _cards;
}

int Hand::size() {
    return _cards.size();
}

bool Hand::empty() {
    return _cards.empty();
}

bool Hand::full() {
    return _cards.size() == MAX_HAND_SIZE;
}
