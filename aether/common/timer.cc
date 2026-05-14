#include <aether/common/timer.hh>

using namespace std::chrono;

namespace ae::timer {

steady_clock::time_point start() {
	return steady_clock::now();
}

long long end(steady_clock::time_point start) {
	steady_clock::time_point const now = steady_clock::now();
	milliseconds const ms              = duration_cast<milliseconds>(now - start);
	return ms.count();
}

} // namespace ae::timer