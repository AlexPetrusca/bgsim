#ifndef BGSIMROOT_CARD_H
#define BGSIMROOT_CARD_H

#include <string>
#include "../../IPrintable.h"

class Card : public IPrintable {
public:
    [[nodiscard]] int id() const;

    [[nodiscard]] std::string name() const;

    void set_name(const std::string &name);

protected:
    int _id{};
    std::string _name;
};


#endif //BGSIMROOT_CARD_H