#include <aether/timer.hh>

namespace aether {

timer::timer()
        : duration_(0) {
}

timer::~timer() = default;

void timer::start() {
	start_timepoint_ = std::chrono::steady_clock::now();
	duration_        = 0;
}

void timer::stop() {
	auto const now = std::chrono::steady_clock::now();
	auto const ms  = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_timepoint_);
	duration_      = ms.count();
}

long long timer::duration() const {
	return duration_;
}

} // namespace aether