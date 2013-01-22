#ifndef TIMER_H
#define TIMER_H

// Determine how timing should occur
#if __cplusplus >= 201103L
// Use C++11 facilities if using C++11
#define CPP11_TIMING
#else
#error "No Timer implemented for current compiler settings"
#endif

#include <ostream>
using std::ostream;

#ifdef CPP11_TIMING
#include <chrono>
typedef std::chrono::system_clock Clock;
#endif


class Timer
{
 public:
    friend ostream& operator<<(ostream& os, Timer& t);

    Timer();
    ~Timer();

    void start();
    void stop();

 private:
#ifdef CPP11_TIMING
    Clock::time_point start_time;
    Clock::time_point stop_time;
#endif
};

#endif
