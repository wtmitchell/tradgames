#include <cstdlib>
#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;

// These are entirely a header portion of boost, no linking needed
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/count.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/min.hpp>
#include <boost/accumulators/statistics/max.hpp>
#include <boost/accumulators/statistics/variance.hpp>
namespace acc = boost::accumulators;
#include <boost/math/distributions/students_t.hpp>
namespace bm = boost::math;

#include "game/GameState.h"
#include "game/Move.h"
#include "game/Players.h"
#include "Math.h"

void collect_random_statistics();
double confidence_bound(double sample_size, double variance, double confidence);
vector<size_t> play_random_game();


int main(int argc, char* argv[])
{

    collect_random_statistics();

    return EXIT_SUCCESS;
}

void collect_random_statistics()
{
    const int num_games = 100000;
    const double conf_bound = 0.95;
    Math::seed_rand();

    acc::accumulator_set<size_t, acc::stats<acc::tag::mean, acc::tag::count,
        acc::tag::variance, acc::tag::min, acc::tag::max> > game_lengths;

    // The bucketed accumulators need a max theoretical limit
    const int max_game_length = 200; // Note divisible by 20, 10, and 5
    acc::accumulator_set<size_t, acc::stats<acc::tag::mean, acc::tag::count,
        acc::tag::variance> > bfactor_all; // All turns
    acc::accumulator_set<size_t, acc::stats<acc::tag::mean, acc::tag::count,
        acc::tag::variance> > bfactor_20[max_game_length/20]; // Groups of 20
    acc::accumulator_set<size_t, acc::stats<acc::tag::mean, acc::tag::count,
        acc::tag::variance> > bfactor_10[max_game_length/10]; // Groups of 10
    acc::accumulator_set<size_t, acc::stats<acc::tag::mean, acc::tag::count,
        acc::tag::variance> > bfactor_5[max_game_length/5]; // Groups of 5

    unsigned int p1_wins = 0;

    cout << "About to play " << num_games << " games of random play." << endl;

    for (int i = 0; i < num_games; ++i)
    {
        // Play a random game
        vector<size_t> b = play_random_game();

        // Record the winner
        if (b.size() % 2 == 1) ++p1_wins;

        // Record the length
        game_lengths(b.size());

        // Record stats about the branching factors
        for (size_t j = 0; j < b.size(); ++j)
        {
            bfactor_all(b[j]);
            bfactor_20[j/20](b[j]);
            bfactor_10[j/10](b[j]);
            bfactor_5[j/5](b[j]);
        }
    }

    double p1_win_percentage = 100 * static_cast<double>(p1_wins) / static_cast<double>(num_games);

    cout << "Done.\n"
         << "All confidence bounds are to " << conf_bound * 100 << "%\n\n"
         << "Player 1 won " << p1_win_percentage << "% of the time ("
         << p1_wins << " wins)\n" << endl;


    cout << "The minimum game length seen was " << acc::min(game_lengths) << " moves\n"
         << "The maximum game length seen was " << acc::max(game_lengths) << " moves\n"
         << "The mean game length is " << acc::mean(game_lengths) << " +/- "
         << confidence_bound(double(acc::count(game_lengths)),
                             acc::variance(game_lengths), conf_bound)
         << "\n" << endl;

    cout << "The mean branching factor across all turns is " << acc::mean(bfactor_all)
         << " +/- " << confidence_bound(double(acc::count(bfactor_all)),
                                        acc::variance(bfactor_all), conf_bound)
         << "\n" << endl;

    cout << "The branching factor in groups of 20:\n";
    for (int i = 0; i < 6; ++i)
    {
        if (acc::count(bfactor_20[i]) > 0)
        {
            cout << i * 20 + 1 << " - " << (i + 1) * 20 << ": " << acc::mean(bfactor_20[i])
                 << " +/- " << confidence_bound(double(acc::count(bfactor_20[i])),
                                                acc::variance(bfactor_20[i]), conf_bound)
                 << "\n";
        }
        else
        {
            cout << i * 20 + 1 << " - " << (i + 1) * 20 << ": No results\n";
        }
    }
    cout << endl;

    cout << "The branching factor in groups of 10:\n";
    for (int i = 0; i < 12; ++i)
    {
        if (acc::count(bfactor_10[i]) > 0)
        {
            cout << i * 10 + 1 << " - " << (i + 1) * 10 << ": " << acc::mean(bfactor_10[i])
                 << " +/- " << confidence_bound(double(acc::count(bfactor_10[i])),
                                                acc::variance(bfactor_10[i]), conf_bound)
                 << "\n";
        }
        else
        {
            cout << i * 10 + 1 << " - " << (i + 1) * 10 << ": No results\n";
        }
    }
    cout << endl;

    cout << "The branching factor in groups of 5:\n";
    for (int i = 0; i < 24; ++i)
    {
        if (acc::count(bfactor_5[i]) > 0)
        {
            cout << i * 5 + 1 << " - " << (i + 1) * 5 << ": " << acc::mean(bfactor_5[i])
                 << " +/- " << confidence_bound(double(acc::count(bfactor_5[i])),
                                                acc::variance(bfactor_5[i]), conf_bound)
                 << "\n";
        }
        else
        {
            cout << i * 5 + 1 << " - " << (i + 1) * 5 << ": No results\n";
        }
    }

    cout << endl;
}

double confidence_bound(double sample_size, double variance, double confidence)
{
    bm::students_t dist(sample_size - 1);
    double T = bm::quantile(bm::complement(dist, (1 - confidence) / 2));
    return T * sqrt(variance) / sqrt(sample_size);
}

// Returns a vector of the possible options at each move
vector<size_t> play_random_game()
{
    vector<size_t> b;

    GameState gs;

    while (!gs.game_over())
    {
        vector<Move> moves = gs.get_moves();
        unsigned int choice = Math::uniform_rand(static_cast<unsigned int>(moves.size()));
        b.push_back(moves.size());
        gs.apply_move(moves[choice]);
    }

    return b;
}

