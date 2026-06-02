#pragma once
#include <chrono>

namespace aether::util {

std::chrono::steady_clock::time_point start();
long long end(std::chrono::steady_clock::time_point start);

} // namespace aether::util