#include <aether/common/log.hh>
#include <fmt/format.h>
#include <fmt/color.h>
#include <fmt/chrono.h>
#include <chrono>

namespace {

std::string time_str() {
	const auto now = std::chrono::system_clock::now();
	const auto seconds = std::chrono::floor<std::chrono::seconds>(now);
	const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - seconds).count();
	return fmt::format("{:%H:%M:%S}.{:03}", seconds, ms);
}

void print_helper(
	std::string_view msg,
	std::string_view level,
	fmt::color level_color,
	std::source_location const& loc
) {
	fmt::print(fmt::fg(fmt::color::gray), "{:<12} ", time_str());
	fmt::print(fmt::fg(level_color), "[{}] ", level);
	fmt::print("{}\n", msg);
}

}

namespace ae::log {

void info(std::string_view msg, std::source_location const& loc) {
	print_helper(msg, "INFO", fmt::color::green_yellow, loc);
}

}