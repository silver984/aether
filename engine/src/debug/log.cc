#include <aether/log.hh>

#if defined(__ae_anydebug__)
	#include <chrono>
	#include <filesystem>
	#include <fmt/chrono.h>
	#include <fstream>
	#include <string>

namespace aether::_log_impl {

namespace fs    = std::filesystem;
using timepoint = std::chrono::time_point<std::chrono::system_clock>;

std::string where_(std::source_location const& loc) {
	fs::path filepath = loc.file_name();
	return fmt::format("{}:{}", filepath.filename().string(), loc.line());
}

std::string time_(timepoint const& now) {
	auto const seconds = std::chrono::floor<std::chrono::seconds>(now);
	return fmt::format("{:%H:%M:%S}", seconds);
}

std::string time_hyph_(timepoint const& now) {
	auto const seconds = std::chrono::floor<std::chrono::seconds>(now);
	return fmt::format("{:%H-%M-%S}", seconds);
}

std::string date_(timepoint const& now) {
	auto const days = std::chrono::floor<std::chrono::days>(now);
	return fmt::format("{:%Y-%m-%d}", days);
}

fs::path log_file;

void print_(std::string_view msg, std::string_view lvl, std::source_location const& loc) {
	timepoint const now       = std::chrono::system_clock::now();
	std::string const log_msg = fmt::format("{:<8} | {:<5} | {:<14} | {}\n", time_(now), lvl, where_(loc), msg);
	std::ofstream file(log_file, std::ios_base::app);
	if (file.is_open()) {
		file << log_msg;
		file.close();
	}
	fmt::print(fmt::runtime(log_msg));
}

bool create_logfile_() {
	fs::create_directories("logs");
	timepoint const now = std::chrono::system_clock::now();
	log_file            = fmt::format("logs/aether_{}_{}.log", date_(now), time_hyph_(now));
	[[maybe_unused]] std::ofstream _(log_file, std::ios::app);
	return fs::exists(log_file);
}

} // namespace aether::_log_impl
#endif