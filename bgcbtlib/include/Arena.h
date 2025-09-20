#ifndef ARENA_H
#define ARENA_H

#include <Board.h>
#include <random>

#include "AnalysisReport.h"
#include "BattleReport.h"
#include "Player.h"

class Arena : public IPrintable {
public:
    Arena(const Player& p1, const Player& p2, std::mt19937 rng = std::mt19937(std::random_device()()));

    static Arena from_boards(const Board& b1, const Board& b2, std::mt19937 rng = std::mt19937(std::random_device()()));

    BattleReport battle(bool debug = false);

    AnalysisReport analyze(int iterations = 10000);

    [[nodiscard]] std::string to_string() override;

private:
    Player p1;
    Player p2;
    std::mt19937 rng;

    BattleStatus get_battle_status();

    void combat(int turn, bool debug = false);
};

#endif //ARENA_H
