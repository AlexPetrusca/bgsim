#ifndef BOARD_H
#define BOARD_H

#include "Minion.h"
#include "../IPrintable.h"

class Board : public IPrintable {
public:
    explicit Board(const std::vector<Minion>& minions = {});

    std::vector<Minion> &get_minions();
    void add_minion(const Minion &minion, size_t idx);
    void remove_minion(size_t idx);
    [[nodiscard]] std::string to_string() const override;
    [[nodiscard]] size_t size() const;
    [[nodiscard]] bool empty() const;
private:
    std::vector<Minion> minions;
    // Aura auras[];
};

#endif //BOARD_H
