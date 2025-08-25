#include "include/Arena.h"

#include <iostream>
#include <random>

Arena::Arena(const Board &boardA, const Board &boardB, std::mt19937 rng) {
    this->boardA = boardA;
    this->boardB = boardB;
    this->rng = rng;
}

BattleStatus Arena::get_battle_status() {
    if (boardA.empty() && boardB.empty()) return TIE;
    if (boardA.empty()) return WIN_B;
    if (boardB.empty()) return WIN_A;

    for (const auto &minion: boardA.get_minions()) {
        if (minion.attack() != 0) return IN_PROGRESS;
    }
    for (const auto &minion: boardB.get_minions()) {
        if (minion.attack() != 0) return IN_PROGRESS;
    }

    return TIE;
}

void debug_combat(Board &attacking, Board &defending, int turn) {
    std::cout << "-------------------------------------------------------------" << std::endl;
    if (turn % 2 == 0) {
        std::cout << attacking << "\n" << defending << std::endl;
    } else {
        std::cout << defending << "\n" << attacking << std::endl;
    }
    std::cout << "-------------------------------------------------------------" << std::endl;
    std::cout << std::endl;
}

BattleReport Arena::battle(bool debug) {
    Board saveA = boardA;
    Board saveB = boardB;

    bool goes_first_A = boardA.size() > boardB.size();
    if (boardA.size() == boardB.size()) {
        goes_first_A = std::uniform_int_distribution(0, 1)(rng);
    }
    Board& attacking = goes_first_A ? boardA : boardB;
    Board& defending = goes_first_A ? boardB : boardA;

    int turn = 0;
    while (get_battle_status() == IN_PROGRESS) {
        std::uniform_int_distribution<size_t> dist_atk(0, attacking.size() - 1);
        std::uniform_int_distribution<size_t> dist_def(0, defending.size() - 1);

        size_t atk_minion_idx = dist_atk(rng);
        size_t def_minion_idx = dist_def(rng);
        Minion &atk_minion = attacking.get_minions().at(atk_minion_idx);
        Minion &def_minion = defending.get_minions().at(def_minion_idx);

        if (debug) {
            std::cout << atk_minion.name() << " -> " << def_minion.name() << std::endl;
            debug_combat(attacking, defending, turn);
        }

        int atk_health = atk_minion.deal_damage(def_minion.attack());
        int def_health = def_minion.deal_damage(atk_minion.attack());
        if (atk_health <= 0) {
            attacking.remove_minion(atk_minion_idx);
        }
        if (def_health <= 0) {
            defending.remove_minion(def_minion_idx);
        }

        std::swap(attacking, defending);
        turn++;
    }

    if (turn % 2 == 1) {
        std::swap(attacking, defending);
    }

    if (debug) {
        std::cout << "Final Board:" << std::endl;;
        debug_combat(attacking, defending, 0);
    }

    BattleStatus status = get_battle_status();
    int damage = 0;
    if (status == WIN_A) {
        damage = boardA.tier_total();
    } else if (status == WIN_B) {
        damage = boardB.tier_total();
    }
    BattleReport report = BattleReport(status, damage);

    boardA = std::move(saveA);
    boardB = std::move(saveB);

    return report;
}

AnalysisReport Arena::analyze(int iterations) {
    AnalysisReport analysis_report = AnalysisReport();
    for (int i = 0; i < iterations; i++) {
        BattleReport battle_report = battle();
        analysis_report.add_battle_report(battle_report);
    }
    return analysis_report;
}