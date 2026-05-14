#ifndef __AETHER_COMMON_TIMER_HH__
#define __AETHER_COMMON_TIMER_HH__

#include <chrono>

namespace ae::timer {

std::chrono::steady_clock::time_point start();
long long end(std::chrono::steady_clock::time_point start);

} // namespace ae::timer

#endif