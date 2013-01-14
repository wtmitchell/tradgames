#ifndef MATH_H
#define MATH_H

namespace Math
{

    unsigned int uniform_rand(const unsigned int max);
    void seed_rand();

    double confidence_bound(double sample_size, double variance, double confidence);
}


#endif
