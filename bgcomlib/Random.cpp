#include "include/util/Random.h"

Random::Random() {
}

int Random::rand_int(const int min, const int max) {
    std::uniform_int_distribution dist(min, max);
    return dist(_rng);
}

int Random::rand_int(const int max) {
    std::uniform_int_distribution dist(0, max);
    return dist(_rng);
}

double Random::rand_double(const double min, const double max) {
    std::uniform_real_distribution dist(min, max);
    return dist(_rng);
}

double Random::rand_double(const double max) {
    std::uniform_real_distribution dist(0.0, max);
    return dist(_rng);
}

bool Random::coin_flip() {
    std::uniform_int_distribution dist(0, 1);
    return dist(_rng);
}

double Random::rand_percent() {
    std::uniform_real_distribution dist(0.0, 1.0);
    return dist(_rng);
}

void Random::seed(const int seed) {
    _rng.seed(seed);
}

Random rng = Random();
