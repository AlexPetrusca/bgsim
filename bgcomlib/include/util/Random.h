#ifndef RANDOM_H
#define RANDOM_H

#include <random>

class Random {
public:
    Random();

    int rand_int(int min, int max);

    int rand_int(int max);

    double rand_double(double min, double max);

    double rand_double(double max);

    bool coin_flip();

    double rand_percent();

    void seed(int seed);

private:
    std::mt19937 _rng;
};

extern Random rng;

#endif //RANDOM_H
