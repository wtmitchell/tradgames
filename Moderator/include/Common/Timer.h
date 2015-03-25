//===------------------------------------------------------------*- C++ -*-===//
///
/// \file
/// \brief This file contains a simple timer wrapping
///
//===----------------------------------------------------------------------===//
#ifndef COMMON_TIMER_H_INCLUDED
#define COMMON_TIMER_H_INCLUDED

#include <ostream>
#include <chrono>

namespace Common {
class Timer {
public:
  friend std::ostream &operator<<(std::ostream &os, Timer &t);

  Timer();

  void start();
  void stop();
  double seconds_elapsed();

private:
  typedef enum { Uninitialized, Invalid, Valid } timer_status;
  timer_status elapsed_valid;

  typedef std::chrono::system_clock Clock;

  Clock::time_point start_time;
  Clock::time_point stop_time;
  Clock::duration elapsed;
};
} // namespace Common
#endif
