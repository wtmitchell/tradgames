#include "Timer.h"

#include <cassert>
#include <iostream>
using std::ostream;
#include <iomanip>
using std::setw;


#if defined(CPP11_TIMING)
#include <chrono>
using std::chrono::duration_cast;
using std::chrono::hours;
using std::chrono::minutes;
using std::chrono::seconds;
using std::chrono::milliseconds;
#endif

#if defined(POSIX_TIMING)
#include <sys/time.h>
#endif


ostream& operator<<(ostream& os, Timer& t)
{
    if (t.elapsed_valid != t.Valid)
    {
        os << "Time information not valid";
        return os;
    }
#if defined(CPP11_TIMING)
    hours hh = duration_cast<hours>(t.elapsed);
    minutes mm = duration_cast<minutes>(t.elapsed - hh);
    seconds ss = duration_cast<seconds>(t.elapsed - hh - mm);
    milliseconds ms = duration_cast<milliseconds>(t.elapsed - hh - mm - ss);

    os << setw(2) << hh.count() << "h "
       << setw(2) << mm.count() << "m "
       << setw(2) << ss.count() << "s "
       << setw(3) << ms.count() << "ms";
#elif defined(POSIX_TIMING)
    time_t hh = t.seconds / (60*60);
    time_t mm = (t.seconds / 60) % 60;
    time_t ss = t.seconds % 60;
    suseconds_t ms = t.useconds / 1000;

    os << setw(2) << hh << "h "
       << setw(2) << mm << "m "
       << setw(2) << ss << "s "
       << setw(3) << ms << "ms";
#endif
    return os;
}

Timer::Timer() : elapsed_valid(Uninitialized)
{}

double Timer::seconds_elapsed()
{
    assert(elapsed_valid == Valid);
#if defined(CPP11_TIMING)
    return static_cast<double>(duration_cast<milliseconds>(elapsed).count()) / 1000;
#elif defined(POSIX_TIMING)
    return seconds + static_cast<double>(useconds) / 1000000;
#endif
}


void Timer::start()
{
    elapsed_valid = Invalid;
#if defined(CPP11_TIMING)
    start_time = Clock::now();
#elif defined(POSIX_TIMING)
    gettimeofday(&start_time, NULL);
#endif
}

void Timer::stop()
{
#if defined(CPP11_TIMING)
    stop_time = Clock::now();
    elapsed = stop_time - start_time;
#elif defined(POSIX_TIMING)
    gettimeofday(&stop_time, NULL);
    seconds = stop_time.tv_sec - start_time.tv_sec;
    useconds = stop_time.tv_usec - start_time.tv_usec;

    // The subtraction could leave negative time which makes no sense, correct it
    if (useconds < 0)
    {
        --seconds;
        useconds += 1000000;
    }
#endif
    // Note this keeps uninitialized values still uninitialized
    if (elapsed_valid == Invalid)
        elapsed_valid = Valid;
}
