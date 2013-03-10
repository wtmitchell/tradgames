//===-- util/Timer.h - A simple timer ---------------------------*- C++ -*-===//
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This file contains a simple timer with timers based on what OS is
/// detected by some preprocessor work.
///
//===----------------------------------------------------------------------===//
#ifndef UTIL_TIMER_H_INCLUDED
#define UTIL_TIMER_H_INCLUDED

// Determine how timing should occur
#if (defined(__cplusplus) && __cplusplus >= 201103L) || (defined(_MSC_VER) && _MSC_VER >= 1700L)
    // Use C++11 std::chrono for timing
    // _MSC_VER >= 1700L implies VS2012 or newer
    // __cplusplus >= 201203L is defined by the C++11 standard
    // it should be correctly defined for clang (3.1+) and gcc (4.7+) when in C++11 mode
    #define CPP11_TIMING
#elif defined(__APPLE__) || defined(__linux__)
    // Fall back to POSIX style gettimeofday timing
    #define POSIX_TIMING
#else
    // Not sure what timer system to use
    // If this #error were commented out, everything should still work, but
    // the timer will do nothing
    #error "No Timer implemented for current compiler settings"
#endif

#include <ostream>
using std::ostream;

#if defined(CPP11_TIMING)
#include <chrono>
typedef std::chrono::system_clock Clock;
#elif defined(POSIX_TIMING)
#include <sys/time.h>
#endif


class Timer
{
 public:
    friend ostream& operator<<(ostream& os, Timer& t);

    Timer();

    void start();
    void stop();
    double seconds_elapsed();

 private:
    typedef enum {Uninitialized, Invalid, Valid} timer_status;
    timer_status elapsed_valid;

#if defined(CPP11_TIMING)
    Clock::time_point start_time;
    Clock::time_point stop_time;
    Clock::duration elapsed;
#elif defined(POSIX_TIMING)
    struct timeval start_time;
    struct timeval stop_time;
    time_t seconds;
    suseconds_t useconds;
#endif
};

#endif
