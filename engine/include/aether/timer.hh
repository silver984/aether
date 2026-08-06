#pragma once
#include <chrono>

namespace aether {

class timer final {
public:
	timer();
	~timer();
	void start();
	void stop();
	long long duration() const;

private:
	std::chrono::steady_clock::time_point start_timepoint_;
	long long duration_;
};

} // namespace aether