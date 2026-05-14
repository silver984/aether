#include <aether/common/log.hh>
#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fmt/chrono.h>
#include <fstream>

namespace {

std::string_view function_name(std::source_location const& loc) {
#ifdef _MSC_VER
	std::string_view func = loc.function_name();

	// cut at '(' early
	if (auto end = func.find('('); end != std::string_view::npos) {
		func = func.substr(0, end);
	}

	// remove trailing spaces
	while (!func.empty() && func.back() == ' ') {
		func.remove_suffix(1);
	}

	// remove calling conventions
	constexpr std::string_view cc_tokens[] = {
		"__cdecl", "__stdcall", "__fastcall", "__vectorcall"};

	for (auto cc : cc_tokens) {
		if (auto pos = func.find(cc); pos != std::string_view::npos) {
			// erase token + surrounding space if present
			auto after = pos + cc.size();
			func.remove_prefix(after < func.size() ? after + 1 : after);
		}
	}

	// 5. trim again (msvc sometimes leaves leading space)
	while (!func.empty() && func.front() == ' ') {
		func.remove_prefix(1);
	}

	return func;
#else
	return loc.function_name();
#endif
}

std::string time_str() {
	auto const now = std::chrono::system_clock::now();
	auto const seconds = std::chrono::floor<std::chrono::seconds>(now);
	auto const ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - seconds).count();
	return fmt::format("{:%H:%M:%S}.{:03}", seconds, ms);
}

std::string file_date_str() {
	auto const now = std::chrono::system_clock::now();
	auto const days = std::chrono::floor<std::chrono::days>(now);
	return fmt::format("{:%Y-%m-%d}", days);
}

std::string file_time_str() {
	auto const now = std::chrono::system_clock::now();
	auto const seconds = std::chrono::floor<std::chrono::seconds>(now);
	return fmt::format("{:%H.%M.%S}", seconds);
}

std::filesystem::path log_file_path;

} // namespace

namespace ae::log::impl {

void print(std::string_view msg, std::string_view level,
		   fmt::color level_color, std::source_location const& loc) {
	std::string time_and_loc_str = fmt::format("{:<12} {} ", time_str(), function_name(loc));
	std::string level_str = fmt::format("[{}] ", level);

	if (!log_file_path.empty()) {
		std::ofstream file(log_file_path, std::ios::app);

		if (file.is_open()) {
			file << fmt::format("{}{}{}\n", time_and_loc_str, level_str, msg);
		}
	}

#ifdef AETHER_DEBUG
	fmt::print(fmt::fg(fmt::color::gray), fmt::runtime(time_and_loc_str));
	fmt::print(fmt::fg(level_color), fmt::runtime(level_str));
	fmt::print("{}\n", msg);
#endif
}

void create_log_file() {
	std::filesystem::create_directory("logs");
	std::string file_name = fmt::format("logs/aether-{}-{}.log", file_date_str(), file_time_str());
	log_file_path = std::filesystem::absolute(file_name);
	std::ofstream file(log_file_path);
	file.close();
}

} // namespace ae::log::impl
