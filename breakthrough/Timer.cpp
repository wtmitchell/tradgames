#include "Timer.h"

#include <iostream>
using std::ostream;
#include <iomanip>
using std::setw;


#ifdef CPP11_TIMING
#include <chrono>
using std::chrono::duration_cast;
using std::chrono::hours;
using std::chrono::minutes;
using std::chrono::seconds;
using std::chrono::milliseconds;
#endif


ostream& operator<<(ostream& os, Timer& t)
{
#ifdef CPP11_TIMING
    Clock::duration elapsed = t.stop_time - t.start_time;
    hours hh = duration_cast<hours>(elapsed);
    minutes mm = duration_cast<minutes>(elapsed - hh);
    seconds ss = duration_cast<seconds>(elapsed - hh - mm);
    milliseconds ms = duration_cast<milliseconds>(elapsed - hh - mm - ss);

    os << setw(2) << hh.count() << "h "
       << setw(2) << mm.count() << "m "
       << setw(2) << ss.count() << "s "
       << setw(3) << ms.count() << "ms";
#endif
    return os;
}

Timer::Timer()
{}
Timer::~Timer()
{}

void Timer::start()
{
#ifdef CPP11_TIMING
    start_time = Clock::now();
#endif
}

void Timer::stop()
{
#ifdef CPP11_TIMING
    stop_time = Clock::now();
#endif
}
