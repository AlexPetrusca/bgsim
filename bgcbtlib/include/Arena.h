#ifndef ARENA_H
#define ARENA_H

#include <Board.h>
#include <random>

#include "AnalysisReport.h"
#include "BattleReport.h"

class Arena {
public:
    Arena(const Board& boardA, const Board& boardB, std::mt19937 rng = std::mt19937(std::random_device()()));

    BattleReport battle(bool debug = false);

    AnalysisReport analyze(int iterations = 10000);

private:
    Board boardA;
    Board boardB;
    std::mt19937 rng;

    BattleStatus get_battle_status();

    void combat(Board& boardA, Board& boardB, int turn, bool debug = false);
};

#endif //ARENA_H
