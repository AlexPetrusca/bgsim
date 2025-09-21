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

    Player* opponent();

    void bind_opponent(Player* opponenet);

    void unbind_opponent();

private:
    Board _board;
    Hand _hand;
    int _health{};
    int _armor{};

    Player* _opponenet;
};

#endif //PLAYER_H
