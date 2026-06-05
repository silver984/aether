#ifdef AETHER_DEBUG
	#include <algorithm>
	#include <chrono>
	#include <filesystem>
	#include <fmt/chrono.h>
	#include <fstream>
	#include <log.hh>

namespace {

std::string_view function_name(std::source_location const& loc) {
	std::string_view func = loc.function_name();

	// remove parameter list
	if (auto paren = func.find('('); paren != std::string_view::npos) {
		func = func.substr(0, paren);
	}

	#ifdef _MSC_VER
	// remove calling conventions
	constexpr std::string_view cc_tokens[] = {"__cdecl", "__stdcall", "__fastcall", "__vectorcall"};

	for (auto cc : cc_tokens) {
		if (auto pos = func.find(cc); pos != std::string_view::npos) {
			auto after = pos + cc.size();

			while (after < func.size() && func[after] == ' ') {
				++after;
			}

			func.remove_prefix(after);
			break;
		}
	}
	#else
	// GCC / Clang / MinGW:
	if (auto space = func.rfind(' '); space != std::string_view::npos) {
		func.remove_prefix(space + 1);
	}
	#endif

	return func;
}

std::string time_str() {
	auto const now     = std::chrono::system_clock::now();
	auto const seconds = std::chrono::floor<std::chrono::seconds>(now);
	auto const ms      = std::chrono::duration_cast<std::chrono::milliseconds>(now - seconds).count();
	return fmt::format("{:%H:%M:%S}.{:03}", seconds, ms);
}

std::string file_date_str() {
	auto const now  = std::chrono::system_clock::now();
	auto const days = std::chrono::floor<std::chrono::days>(now);
	return fmt::format("{:%Y-%m-%d}", days);
}

std::string file_time_str() {
	auto const now     = std::chrono::system_clock::now();
	auto const seconds = std::chrono::floor<std::chrono::seconds>(now);
	return fmt::format("{:%H.%M.%S}", seconds);
}

std::filesystem::path log_file_path;

} // namespace

namespace aether::log::__impl {

void __print(std::string_view msg, std::string_view level, fmt::color level_color, std::source_location const& loc) {
	std::string time_and_loc_str = fmt::format("{:<12} {} ", time_str(), function_name(loc));
	std::string level_str        = fmt::format("[{}] ", level);

	if (!log_file_path.empty()) {
		std::ofstream file(log_file_path, std::ios::app);

		if (file.is_open()) {
			file << fmt::format("{}{}{}\n", time_and_loc_str, level_str, msg);
		}
	}

	fmt::print(fmt::fg(fmt::color::gray), fmt::runtime(time_and_loc_str));
	fmt::print(fmt::fg(level_color), fmt::runtime(level_str));
	fmt::print("{}\n", msg);
}

void __create_log_file() {
	std::filesystem::create_directory("logs");
	std::string file_name = fmt::format("logs/aether-{}-{}.log", file_date_str(), file_time_str());
	log_file_path         = std::filesystem::absolute(file_name);
	std::ofstream file(log_file_path);
	file.close();
}

} // namespace aether::log::__impl
#endif