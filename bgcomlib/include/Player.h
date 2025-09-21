#ifndef PLAYER_H
#define PLAYER_H

#include "Board.h"
#include "Hand.h"

class Player {
public:
    Player();

    [[nodiscard]] Board& board();

    void set_board(Board& board);

    [[nodiscard]] Hand& hand();

    [[nodiscard]] int health() const;

    [[nodiscard]] int armor() const;

    int total_health();

    void deal_damage(int damage);

    void set_health(int health);

    void set_armor(int armor);

private:
    Board _board;
    Hand _hand;
    int _health{};
    int _armor{};
};

#endif //PLAYER_H
