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

    for (const auto& minion: boardA.minions()) {
        if (minion.attack() != 0) return IN_PROGRESS;
    }
    for (const auto& minion: boardB.minions()) {
        if (minion.attack() != 0) return IN_PROGRESS;
    }

    return TIE;
}

// todo: this can be arena's to_string()
void debug_combat(Board& boardA, Board& boardB) {
    std::cout << "-------------------------------------------------------------" << std::endl;
    std::cout << "[A] " << boardA << std::endl;
    std::cout << "[B] " << boardB << std::endl;
    std::cout << "-------------------------------------------------------------" << std::endl;
}

void Arena::combat(Board& boardA, Board& boardB, const int turn, const bool debug) {
    Board& attacking = turn % 2 == 0 ? boardA : boardB;
    Board& defending = turn % 2 == 0 ? boardB : boardA;

    // select attacker
    const MinionLoc atk_minion = attacking.active();

    int attack_count = 1;
    if (atk_minion->has(Keyword::WINDFURY)) {
        attack_count = 2;
    } else if (atk_minion->has(Keyword::MEGA_WINDFURY)) {
        attack_count = 4;
    }

    bool attacker_died = false;
    for (int i = 0; i < attack_count; i++) {
        // select defender
        MinionLoc def_minion;
        if (defending.taunt_count() > 0) {
            std::uniform_int_distribution<size_t> dist_def(0, defending.taunt_count() - 1);
            size_t taunt_idx = dist_def(rng);
            for (auto m = defending.minions().begin(); m != defending.minions().end(); ++m) {
                if (m->has(Keyword::TAUNT)) {
                    if (taunt_idx == 0) {
                        def_minion = m;
                        break;
                    }
                    taunt_idx--;
                }
            }
        } else {
            std::uniform_int_distribution<size_t> dist_def(0, defending.size() - 1);
            const size_t minion_idx = dist_def(rng);
            def_minion = std::next(defending.minions().begin(), minion_idx);
        }

        if (debug) {
            if (turn % 2 == 0) {
                std::cout << "[A -> B] ";
            } else {
                std::cout << "[B -> A] ";
            }
            // std::cout << atk_minion->name() << " (" << atk_minion_idx << ")"
            //     << " -> "
            //     << def_minion->name() << " (" << def_minion_idx << ")" << std::endl;
            std::cout << "\"" << atk_minion->name() << "\"" << " -> " << "\"" << def_minion->name() << "\"" << std::endl;
            debug_combat(boardA, boardB);
        }

        const int atk_attack = atk_minion->attack();
        const int def_attack = def_minion->attack();
        attacker_died = attacking.damage_minion(atk_minion, def_attack);
        defending.damage_minion(def_minion, atk_attack);

        if (attacker_died) {
            break;
        }
    }

    if (!attacker_died) {
        attacking.increment_active();
    }

    if (debug) {
        debug_combat(boardA, boardB);
        std::cout << std::endl;
    }
}

BattleReport Arena::battle(const bool debug) {
    // save
    Board saveA = boardA;
    Board saveB = boardB;

    int turn = boardA.size() > boardB.size() ? 0 : 1;
    if (boardA.size() == boardB.size()) {
        turn = std::uniform_int_distribution(0, 1)(rng);
    }

    boardA.prep_for_battle();
    boardB.prep_for_battle();
    while (get_battle_status() == IN_PROGRESS) {
        combat(boardA, boardB, turn, debug);
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
