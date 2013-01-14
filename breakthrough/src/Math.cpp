#include <cstdlib>
using std::rand;
using std::srand;
#include <ctime>
using std::time;

#include "Math.h"

void Math::seed_rand()
{
    srand(static_cast<unsigned int>(time(0)));
}

unsigned int Math::uniform_rand(const unsigned int max)
{
    /* Returns a random integer in [0, max-1] uniformly
     *
     * This clamp procedure is necessary to avoid modulo bias.
     * In particular the results of rand are discarded if they
     * would result in a non-uniform distribution.
     */

    unsigned int result = RAND_MAX;

    const unsigned int rand_limit = RAND_MAX - (RAND_MAX % max);

    while(result > rand_limit)
    {
        result = rand();
    }

    return result % max;
}

