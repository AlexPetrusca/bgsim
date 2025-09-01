#include "include/Arena.h"

#include <iostream>
#include <random>
#include <sstream>

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
            std::cout << this->to_string();
        }

        // resolve damage
        attacking.damage_minion(atk_minion, def_minion->attack());
        if (atk_minion->has(Keyword::CLEAVE)) {
            // attack left
            if (def_minion != defending.minions().begin()) { // if not attacking head
                const MinionLoc left_def_minion = std::prev(def_minion);
                defending.damage_minion(left_def_minion, atk_minion->attack());
            }
            // attack middle
            defending.damage_minion(def_minion, atk_minion->attack());
            // attack right
            const MinionLoc right_def_minion = std::next(def_minion);
            if (right_def_minion != defending.minions().end()) { // if not attacking tail
                defending.damage_minion(right_def_minion, atk_minion->attack());
            }
        } else {
            defending.damage_minion(def_minion, atk_minion->attack());
        }

        // resolve deaths
        const bool is_cleave = atk_minion->has(Keyword::CLEAVE);
        // todo: we should return something else from `reap` & `try_reap`
        //  - certain effects like "blaster" and "the beast" need to be handled specially by it
        attacker_died = attacking.try_reap_minion(atk_minion);
        if (is_cleave) {
            // todo: trying to reap all is inefficient
            auto m = defending.minions().begin();
            while (m != defending.minions().end()) {
                const auto m_next = std::next(m);
                defending.try_reap_minion(m);
                m = m_next;
            }
        } else {
            defending.try_reap_minion(def_minion);
        }

        if (attacker_died) {
            break;
        }
    }

    if (!attacker_died) {
        attacking.increment_active();
    }

    if (debug) {
        std::cout << this->to_string();
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
        std::cout << this->to_string();
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

std::string Arena::to_string() {
    std::ostringstream oss;
    oss << "-------------------------------------------------------------" << std::endl;
    oss << "[A] " << boardA << std::endl;
    oss << "[B] " << boardB << std::endl;
    oss << "-------------------------------------------------------------" << std::endl;
    return oss.str();
}
