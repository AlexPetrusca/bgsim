#include "include/Arena.h"

#include <iostream>
#include <random>
#include <sstream>

#include "util/Random.h"

Arena::Arena(const Player& p1, const Player& p2) {
    this->_p1 = p1;
    this->_p2 = p2;
}

Arena Arena::from_boards(const Board& b1, const Board& b2) {
    Player p1;
    Player p2;
    p1.set_board(b1);
    p2.set_board(b2);
    return Arena(p1, p2);
}

BattleStatus Arena::get_battle_status() {
    if (_p1.board().empty() && _p2.board().empty()) return TIE;
    if (_p1.board().empty()) return WIN_B;
    if (_p2.board().empty()) return WIN_A;

    for (const auto& minion: _p1.board().minions()) {
        if (minion.attack() != 0) return IN_PROGRESS;
    }
    for (const auto& minion: _p2.board().minions()) {
        if (minion.attack() != 0) return IN_PROGRESS;
    }

    return TIE;
}

void fight_minions(Board& attacking, Board& defending, const MinionLoc atk, const MinionLoc def) {
    const bool def_poisoned = def->has(Keyword::POISONOUS) || def->has(Keyword::VENOMOUS);
    const int def_damage_dealt = attacking.damage_minion(atk, def->attack(), def_poisoned);
    if (def_damage_dealt > 0 && def->has(Keyword::VENOMOUS)) {
        def->clear(Keyword::VENOMOUS);
    }

    const bool atk_poisoned = atk->has(Keyword::POISONOUS) || atk->has(Keyword::VENOMOUS);
    int atk_damage_dealt = 0;
    // todo: rewrite using `get_left_minion_loc` and `get_right_minion_loc`
    if (atk->has(Keyword::CLEAVE)) {
        // attack left
        if (def != defending.minions().begin()) { // if not attacking head
            const auto ours_left = std::prev(def);
            atk_damage_dealt += defending.damage_minion(ours_left, atk->attack(), atk_poisoned);
        }
        // attack middle
        atk_damage_dealt += defending.damage_minion(def, atk->attack(), atk_poisoned);
        // attack right
        const auto ours_right = std::next(def);
        if (ours_right != defending.minions().end()) { // if not attacking tail
            atk_damage_dealt += defending.damage_minion(ours_right, atk->attack(), atk_poisoned);
        }
    } else {
        atk_damage_dealt = defending.damage_minion(def, atk->attack(), atk_poisoned);
    }
    if (atk_damage_dealt > 0 && atk->has(Keyword::VENOMOUS)) {
        atk->clear(Keyword::VENOMOUS);
    }
}

void Arena::combat(const int turn, const bool debug) {
    Board& attacking = turn % 2 == 0 ? _p1.board() : _p2.board();
    Board& defending = turn % 2 == 0 ? _p2.board() : _p1.board();

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
            int taunt_idx = rng.rand_int(defending.taunt_count() - 1);
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
            const int minion_idx = rng.rand_int(defending.size() - 1);
            def_minion = std::next(defending.minions().begin(), minion_idx);
        }

        if (debug) {
            std::cout << "|Turn " << turn << "| ";
            if (turn % 2 == 0) {
                std::cout << "[A -> B] ";
            } else {
                std::cout << "[B -> A] ";
            }
            int atk_minion_idx = -1;
            for (auto m = attacking.minions().begin(); m != attacking.minions().end(); ++m) {
                atk_minion_idx++;
                if (m == atk_minion) {
                    break;
                }
            }
            int def_minion_idx = -1;
            for (auto m = defending.minions().begin(); m != defending.minions().end(); ++m) {
                def_minion_idx++;
                if (m == def_minion) {
                    break;
                }
            }
            std::cout << "\"" << atk_minion->name() << "\"" << " (" << atk_minion_idx << ")";
            std::cout << " -> ";
            std::cout << "\"" << def_minion->name() << "\"" << " (" << def_minion_idx << ")";
            std::cout << std::endl;
            std::cout << this->to_string();
        }

        // resolve damage
        fight_minions(attacking, defending, atk_minion, def_minion);

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
    Board saveA = _p1.board();
    Board saveB = _p2.board();

    int turn = _p1.board().size() > _p2.board().size() ? 0 : 1;
    if (_p1.board().size() == _p2.board().size()) {
        turn = rng.coin_flip();
    }

    if (debug) {
        std::cout << std::endl;
    }

    _p1.board().pre_battle();
    _p2.board().pre_battle();
    while (get_battle_status() == IN_PROGRESS) {
        _p1.board().pre_combat();
        _p2.board().pre_combat();
        combat(turn, debug);
        _p1.board().post_combat();
        _p2.board().post_combat();
        turn++;
    }

    if (debug) {
        std::cout << "Final Board:" << std::endl;;
        std::cout << this->to_string();
    }

    const BattleStatus status = get_battle_status();
    int damage = 0;
    if (status == WIN_A) {
        damage = _p1.board().tier_total();
    } else if (status == WIN_B) {
        damage = _p2.board().tier_total();
    }
    const BattleReport report = BattleReport(status, damage);

    // restore
    _p1.board() = std::move(saveA);
    _p2.board() = std::move(saveB);

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
    oss << "[A] " << _p1.board() << std::endl;
    oss << "[B] " << _p2.board() << std::endl;
    oss << "-------------------------------------------------------------" << std::endl;
    return oss.str();
}
