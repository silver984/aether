#include <aether/common/timer.hh>

namespace ae::timer {

std::chrono::steady_clock::time_point start() {
	return std::chrono::high_resolution_clock::now();
}

long long end(std::chrono::steady_clock::time_point const& start) {
	std::chrono::steady_clock::time_point const now = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds const ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
	return ms.count();
}

}