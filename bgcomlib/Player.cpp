#include "include/Player.h"

Player::Player() {
    _health = 30;
    _armor = 10;
    _board.bind_player(this);
}

Board& Player::board() {
    return _board;
}

void Player::set_board(const Board& board) {
    _board = board;
    _board.bind_player(this);
}

Hand& Player::hand() {
    return _hand;
}

int Player::health() const {
    return _health;
}

int Player::armor() const {
    return _armor;
}

int Player::total_health() {
    return _health + _armor;
}

void Player::deal_damage(const int damage) {
    if (damage < _armor) {
        _armor -= damage;
    } else {
        _health -= damage - _armor;
        _armor = 0;
    }
}

void Player::set_health(const int health) {
    _health = health;
}

void Player::set_armor(const int armor) {
    _armor = armor;
}
