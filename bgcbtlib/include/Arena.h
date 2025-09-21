#ifndef ARENA_H
#define ARENA_H

#include <Board.h>
#include <random>

#include "AnalysisReport.h"
#include "BattleReport.h"
#include "Player.h"

class Arena : public IPrintable {
public:
    Arena();

    Arena(const Player& p1, const Player& p2);

    static Arena from_boards(Board& b1, Board& b2);

    BattleReport battle(bool debug = false, bool analysis = false);

    AnalysisReport analyze(int iterations = 10000);

    [[nodiscard]] Player& playerA();

    [[nodiscard]] Player& playerB();

    [[nodiscard]] std::string to_string() override;

private:
    Player _p1;
    Player _p2;

    BattleStatus get_battle_status();

    void combat(int turn, bool debug = false);
};

#endif //ARENA_H
