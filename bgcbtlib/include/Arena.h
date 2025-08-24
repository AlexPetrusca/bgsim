#ifndef ARENA_H
#define ARENA_H

#include <Board.h>
#include <random>

class Arena {
public:
    Arena(const Board& boardA, const Board& boardB, std::mt19937 rng);
    void battle(); // todo: return battle report
private:
    Board boardA;
    Board boardB;
    std::mt19937 rng;
    bool isTerminal();
};

#endif //ARENA_H
