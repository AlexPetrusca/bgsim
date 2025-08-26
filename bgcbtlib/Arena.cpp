#include "include/Arena.h"

#include <iostream>
#include <random>

Arena::Arena(const Board& boardA, const Board& boardB, std::mt19937 rng) {
    this->boardA = boardA;
    this->boardB = boardB;
    this->rng = rng;
}

BattleStatus Arena::get_battle_status() {
    if (boardA.empty() && boardB.empty()) return TIE;
    if (boardA.empty()) return WIN_B;
    if (boardB.empty()) return WIN_A;

    for (const auto& minion: boardA.get_minions()) {
        if (minion.attack() != 0) return IN_PROGRESS;
    }
    for (const auto& minion: boardB.get_minions()) {
        if (minion.attack() != 0) return IN_PROGRESS;
    }

    return TIE;
}

void debug_combat(Board& boardA, Board& boardB) {
    std::cout << "-------------------------------------------------------------" << std::endl;
    std::cout << "[A] " << boardA << std::endl;
    std::cout << "[B] " << boardB << std::endl;
    std::cout << "-------------------------------------------------------------" << std::endl;
    std::cout << std::endl;
}

int Arena::combat(Board& boardA, Board& boardB, const int turn, const int attack_idx, const bool debug) {
    Board& attacking = turn % 2 == 0 ? boardA : boardB;
    Board& defending = turn % 2 == 0 ? boardB : boardA;

    const size_t atk_minion_idx = attack_idx % attacking.get_minions().size();
    Minion& atk_minion = attacking.get_minions().at(atk_minion_idx);

    std::uniform_int_distribution<size_t> dist_def(0, defending.size() - 1);
    const size_t def_minion_idx = dist_def(rng);
    Minion& def_minion = defending.get_minions().at(def_minion_idx);

    if (debug) {
        if (turn % 2 == 0) {
            std::cout << "[A -> B] ";
        } else {
            std::cout << "[B -> A] ";
        }
        std::cout << atk_minion.name() << " -> " << def_minion.name() << std::endl;
        debug_combat(boardA, boardB);
    }

    const int atk_health = atk_minion.deal_damage(def_minion.attack());
    const int def_health = def_minion.deal_damage(atk_minion.attack());
    bool attacker_died = false;
    if (atk_health <= 0) {
        attacking.kill_minion(atk_minion_idx);
        attacker_died = true;
    }
    if (def_health <= 0) {
        defending.kill_minion(def_minion_idx);
    }
    return attacker_died ? attack_idx : attack_idx + 1;
}

BattleReport Arena::battle(const bool debug) {
    // save
    Board saveA = boardA;
    Board saveB = boardB;

    int turn = boardA.size() > boardB.size() ? 0 : 1;
    if (boardA.size() == boardB.size()) {
        turn = std::uniform_int_distribution(0, 1)(rng);
    }

    int atk_idx_a = 0;
    int atk_idx_b = 0;
    while (get_battle_status() == IN_PROGRESS) {
        if (turn % 2 == 0) {
            atk_idx_a = combat(boardA, boardB, turn, atk_idx_a, debug);
        } else {
            atk_idx_b = combat(boardA, boardB, turn, atk_idx_b, debug);
        }
        turn++;
    }

    if (debug) {
        std::cout << "Final Board:" << std::endl;;
        debug_combat(boardA, boardB);
    }

    const BattleStatus status = get_battle_status();
    int damage = 0;
    if (status == WIN_A) {
        damage = boardA.tier_total();
    } else if (status == WIN_B) {
        damage = boardB.tier_total();
    }
    const BattleReport report = BattleReport(status, damage);

    // restore
    boardA = std::move(saveA);
    boardB = std::move(saveB);

    return report;
}

AnalysisReport Arena::analyze(int iterations) {
    AnalysisReport analysis_report = AnalysisReport();
    for (int i = 0; i < iterations; i++) {
        const BattleReport battle_report = battle();
        analysis_report.add_battle_report(battle_report);
    }
    return analysis_report;
}
