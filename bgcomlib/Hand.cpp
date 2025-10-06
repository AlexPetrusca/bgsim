#include "include/Hand.h"

Hand::Hand() {
}

bool Hand::add_card(const Minion& minion) {
    if (size() == MAX_HAND_SIZE) return false;

    _cards.push_back(minion);
    return true;
}

bool Hand::add_card(CardDb::Id id) {
    return add_card(db.get_minion(id));
}

void Hand::remove_card(const MinionLoc loc) {
    _cards.erase(loc);
}

void Hand::remove_card(const int index) {
    if (index >= _cards.size()) return;

    MinionLoc loc = _cards.begin();
    for (int i = 0; i < index; i++) {
        ++loc;
    }
    remove_card(loc);
}

MinionLoc Hand::begin() {
    return _cards.begin();
}

MinionLoc Hand::end() {
    return _cards.end();
}


std::list<Minion>& Hand::cards() {
    return _cards;
}

int Hand::size() {
    return _cards.size();
}
