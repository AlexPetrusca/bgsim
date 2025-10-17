#include "card/Card.h"

int Card::id() const {
    return _id;
}

std::string Card::name() const {
    return _name;
}

void Card::set_name(const std::string& name) {
    this->_name = name;
}
