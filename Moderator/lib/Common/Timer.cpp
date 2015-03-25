//===------------------------------------------------------------*- C++ -*-===//
#include "Common/Timer.h"

#include <cassert>
#include <iostream>
using std::ostream;
#include <iomanip>
using std::setw;
#include <chrono>
using std::chrono::duration_cast;
using std::chrono::hours;
using std::chrono::minutes;
using std::chrono::seconds;
using std::chrono::milliseconds;

namespace Common {
std::ostream &operator<<(std::ostream &os, Timer &t) {
  if (t.elapsed_valid != t.Valid) {
    os << "Time information not valid";
    return os;
  }
  hours hh = duration_cast<hours>(t.elapsed);
  minutes mm = duration_cast<minutes>(t.elapsed - hh);
  seconds ss = duration_cast<seconds>(t.elapsed - hh - mm);
  milliseconds ms = duration_cast<milliseconds>(t.elapsed - hh - mm - ss);

  os << setw(2) << hh.count() << "h " << setw(2) << mm.count() << "m "
     << setw(2) << ss.count() << "s " << setw(3) << ms.count() << "ms";
  return os;
}

Timer::Timer() : elapsed_valid(Uninitialized) {}

double Timer::seconds_elapsed() {
  assert(elapsed_valid == Valid);
  return static_cast<double>(duration_cast<milliseconds>(elapsed).count()) /
         1000;
}

void Timer::start() {
  elapsed_valid = Invalid;
  start_time = Clock::now();
}

void Timer::stop() {
  stop_time = Clock::now();
  elapsed = stop_time - start_time;
  // Note this keeps uninitialized values still uninitialized
  if (elapsed_valid == Invalid)
    elapsed_valid = Valid;
}
} // namespace Common
