#include <util/timer.hh>

namespace aether::util {

std::chrono::steady_clock::time_point start() {
	return std::chrono::steady_clock::now();
}

long long end(std::chrono::steady_clock::time_point start) {
	std::chrono::steady_clock::time_point const now = std::chrono::steady_clock::now();
	std::chrono::milliseconds const ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
	return ms.count();
}

} // namespace aether::util