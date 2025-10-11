#ifndef PLAYER_H
#define PLAYER_H

#include "Board.h"
#include "Discovers.h"
#include "Hand.h"
#include "Pool.h"

class Player {
public:
    Player();

    [[nodiscard]] Board& board();

    void set_board(Board& board);

    [[nodiscard]] Hand& hand();

    [[nodiscard]] Discovers& discovers();

    [[nodiscard]] int health() const;

    [[nodiscard]] int armor() const;

    int missing_health() const;

    int total_health();

    void deal_damage(int damage);

    void set_health(int health);

    void set_armor(int armor);

    void increment_pogo_counter();

    int pogo_counter();

    bool is_immune();

    void set_immune(bool is_immune);

    void set_tier(int tier);

    int tier();

    void select_discover(int idx);

    Player* opponent();

    Pool* pool();

    void bind_opponent(Player* opponenet);

    void bind_pool(Pool* pool);

    void unbind_opponent();

private:
    Board _board;
    Hand _hand;
    Discovers _discovers;
    int _health{};
    int _max_health{};
    int _armor{};
    int _tier{1};

    Player* _opponenet{};
    Pool* _pool{};
    int _pogo_counter{}; // todo: should this be on the board instead? probably...
    bool _is_immune{};
};

#endif //PLAYER_H
