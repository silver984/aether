#include <aether/common/log.hh>
#include <fmt/chrono.h>
#include <chrono>

namespace {

std::string_view function_name(std::source_location const& loc) {
	std::string_view func = loc.function_name();

	// remove params
	// cut at '(' first
	if (
		auto end = func.find('(');
		end != std::string_view::npos
	) {
		func = func.substr(0, end);
	}

	// remove trailing whitespace
	while (!func.empty() && func.back() == ' ') {
		func.remove_suffix(1);
	}

	// find last space
	// function name starts after it
	if (
		auto start = func.find_last_of(' ');
		start != std::string_view::npos
	) {
		func = func.substr(start + 1);
	}

	return func;
}

std::string time_str() {
	const auto now = std::chrono::system_clock::now();
	const auto seconds = std::chrono::floor<std::chrono::seconds>(now);
	const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - seconds).count();
	return fmt::format("{:%H:%M:%S}.{:03}", seconds, ms);
}

}

namespace ae::log::impl {

void print(std::string_view msg, std::string_view level, fmt::color level_color, std::source_location const& loc) {
	fmt::print(fmt::fg(fmt::color::gray), "{:<12} {} ", time_str(), function_name(loc));
	fmt::print(fmt::fg(level_color), "[{}] ", level);
	fmt::print("{}\n", msg);
}

}
