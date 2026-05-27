#pragma once
#include <chrono>

namespace ae::util::timer {

std::chrono::steady_clock::time_point start();
long long end(std::chrono::steady_clock::time_point start);

} // namespace ae::util::timer