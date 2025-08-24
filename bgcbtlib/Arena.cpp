#include "include/Arena.h"

#include <iostream>
#include <random>

Arena::Arena(const Board &boardA, const Board &boardB, std::mt19937 rng = std::mt19937(std::random_device()())) {
    this->boardA = boardA;
    this->boardB = boardB;
    this->rng = rng;
}

bool Arena::isTerminal() {
    if (boardA.empty()) return true;
    if (boardB.empty()) return true;

    for (const auto &minion: boardA.get_minions()) {
        if (minion.attack() != 0) return false;
    }
    for (const auto &minion: boardB.get_minions()) {
        if (minion.attack() != 0) return false;
    }

    return true;
}

void Arena::battle() {
    Board saveA = boardA;
    Board saveB = boardB;

    int goes_first_rand = std::uniform_int_distribution(0, 1)(rng);
    Board& attacking = goes_first_rand == 0 ? boardA : boardB;
    Board& defending = goes_first_rand == 0 ? boardB : boardA;
    int turn = 0;
    while (!isTerminal()) {
        std::uniform_int_distribution<size_t> distAtk(0, attacking.size() - 1);
        std::uniform_int_distribution<size_t> distDef(0, defending.size() - 1);

        size_t atk_minion_idx = distAtk(rng);
        size_t def_minion_idx = distDef(rng);
        Minion &atk_minion = attacking.get_minions().at(atk_minion_idx);
        Minion &def_minion = defending.get_minions().at(def_minion_idx);

        std::cout << std::endl;
        std::cout << atk_minion.name() << " -> " << def_minion.name() << std::endl;
        std::cout << "-------------------------------------------------------------" << std::endl;
        if (turn % 2 == 0) {
            std::cout << attacking << "\n" << defending << std::endl;
        } else {
            std::cout << defending << "\n" << attacking << std::endl;
        }
        std::cout << "-------------------------------------------------------------" << std::endl;

        int atk_health = atk_minion.deal_damage(def_minion.attack());
        if (atk_health <= 0) {
            attacking.remove_minion(atk_minion_idx);
        }
        int def_health = def_minion.deal_damage(atk_minion.attack());
        if (def_health <= 0) {
            defending.remove_minion(def_minion_idx);
        }

        std::swap(attacking, defending);
        turn++;
    }

    std::cout << std::endl;
    std::cout << "Final Board:" << std::endl;;
    std::cout << "-------------------------------------------------------------" << std::endl;
    if (turn % 2 == 0) {
        std::cout << attacking << "\n" << defending << std::endl;
    } else {
        std::cout << defending << "\n" << attacking << std::endl;
    }
    std::cout << "-------------------------------------------------------------" << std::endl;

    boardA = std::move(saveA);
    boardB = std::move(saveB);
}
