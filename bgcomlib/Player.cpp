#include "include/Player.h"

Player::Player() {
    _board.bind_player(this);
    _health = 30;
    _armor = 10;
}

Board& Player::board() {
    return _board;
}

void Player::set_board(const Board& board) {
    _board = board;
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

void Player::set_health(const int health) {
    _health = health;
}

void Player::set_armor(const int armor) {
    _armor = armor;
}
