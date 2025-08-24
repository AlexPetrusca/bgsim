#ifndef BOARD_H
#define BOARD_H

#include "Minion.h"
#include "../IPrintable.h"

class Board : public IPrintable {
public:
    explicit Board(const std::vector<Minion>& minions = {});
    std::vector<Minion> get_minions();
    void add_minion(const Minion &minion, int idx);
    [[nodiscard]] std::string to_string() const override;
private:
    std::vector<Minion> minions;
    // Aura auras[];
};

#endif //BOARD_H
