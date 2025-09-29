#ifndef PLAYER_H
#define PLAYER_H

#include "Board.h"
#include "Hand.h"
#include "Pool.h"

class Player {
public:
    Player();

    [[nodiscard]] Board& board();

    void set_board(Board& board);

    [[nodiscard]] Hand& hand();

    [[nodiscard]] int health() const;

    [[nodiscard]] int armor() const;

    int missing_health() const;

    int total_health();

    void deal_damage(int damage);

    void set_health(int health);

    void set_armor(int armor);

    void increment_pogo_counter();

    int pogo_counter();

    Player* opponent();

    Pool* pool();

    void bind_opponent(Player* opponenet);

    void bind_pool(Pool* pool);

    void unbind_opponent();

private:
    Board _board;
    Hand _hand;
    int _health{};
    int _max_health{};
    int _armor{};

    Player* _opponenet{};
    Pool* _pool{};
    int _pogo_counter{};
};

#endif //PLAYER_H
