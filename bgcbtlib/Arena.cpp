#include "include/Arena.h"

#include <iostream>
#include <random>

Arena::Arena(const Board& boardA, const Board& boardB, std::mt19937 rng = std::mt19937(std::random_device()())) {
    this->boardA = boardA;
    this->boardB = boardB;
    this->rng = rng;
}

bool Arena::isTerminal() {
    if (boardA.get_minions().empty()) return true;
    if (boardB.get_minions().empty()) return true;

    for (const auto& minion : boardA.get_minions()) {
        if (minion.attack() != 0) return false;
    }
    for (const auto& minion : boardB.get_minions()) {
        if (minion.attack() != 0) return false;
    }

    return true;
}

void Arena::battle() {
    Board board1 = boardA;
    Board board2 = boardB;

    int goesFirstRng = std::uniform_int_distribution(0, 1)(rng);
    Board attacking = goesFirstRng == 0 ? board1 : board2;
    Board defending = goesFirstRng == 0 ? board2 : board1;
    while (!isTerminal()) {
        std::swap(attacking, defending);
        std::cout << board1 << "\n" << board2 << "\n" << std::endl;
    }

    std::cout << "Final Board:\n" << board1 << "\n" << board2 << std::endl;
}
