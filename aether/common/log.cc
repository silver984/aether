#include <aether/common/log.hh>
#include <fmt/format.h>
#include <fmt/color.h>
#include <fmt/chrono.h>
#include <chrono>

namespace {

std::string_view function_name(std::source_location const& location) {
	std::string_view func = location.function_name();

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

void print_helper(
	std::string_view msg,
	std::string_view level,
	fmt::color level_color,
	std::source_location const& loc
) {
	fmt::print(fmt::fg(fmt::color::gray), "{:<12} {} ", time_str(), function_name(loc));
	fmt::print(fmt::fg(level_color), "[{}] ", level);
	fmt::print("{}\n", msg);
}

}

namespace ae::log {

void trace(std::string_view msg, std::source_location const& loc) {
	print_helper(msg, "TRACE", fmt::color::dark_turquoise, loc);
}

void debug(std::string_view msg, std::source_location const& loc) {
	print_helper(msg, "DEBUG", fmt::color::medium_violet_red, loc);
}

void info(std::string_view msg, std::source_location const& loc) {
	print_helper(msg, "INFO", fmt::color::green_yellow, loc);
}

void warn(std::string_view msg, std::source_location const& loc) {
	print_helper(msg, "WARNING", fmt::color::gold, loc);
}

void error(std::string_view msg, std::source_location const& loc) {
	print_helper(msg, "WARNING", fmt::color::crimson, loc);
}

}