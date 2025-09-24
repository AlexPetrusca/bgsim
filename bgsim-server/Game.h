#ifndef BGSIMROOT_GAME_H
#define BGSIMROOT_GAME_H

#include <vector>

#include "Player.h"
#include "Pool.h"

class Game {
public:
    Game();

private:
    std::vector<Player> _players;
    Pool _pool;
};

#endif //BGSIMROOT_GAME_H