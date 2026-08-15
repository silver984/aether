#include <aether/log.hh>
#if defined(AETHER_ENGINE_HAS_DEBUG)
	#include <chrono>
	#include <filesystem>
	#include <fmt/chrono.h>
	#include <fstream>
	#include <string>

using timepoint = std::chrono::time_point<std::chrono::system_clock>;
namespace fs    = std::filesystem;

namespace {

std::string where(std::source_location const& loc) {
	fs::path filepath = loc.file_name();
	return fmt::format("{}:{}", filepath.filename().string(), loc.line());
}

std::string time(timepoint const& now) {
	auto const seconds = std::chrono::floor<std::chrono::seconds>(now);
	return fmt::format("{:%H:%M:%S}", seconds);
}

std::string timehyph(timepoint const& now) {
	auto const seconds = std::chrono::floor<std::chrono::seconds>(now);
	return fmt::format("{:%H-%M-%S}", seconds);
}

std::string date(timepoint const& now) {
	auto const days = std::chrono::floor<std::chrono::days>(now);
	return fmt::format("{:%Y-%m-%d}", days);
}

fs::path logfile;

} // namespace

namespace aether::_log_impl {

void print(std::string_view msg, std::string_view lvl, std::source_location const& loc) {
	timepoint const now       = std::chrono::system_clock::now();
	std::string const log_msg = fmt::format("{:<8} | {:<5} | {:<14} | {}", time(now), lvl, where(loc), msg);

	std::ofstream file(logfile, std::ios_base::app);
	if (file.is_open()) {
		file << log_msg << "\n";
		file.close();
	}

	fmt::println(fmt::runtime(log_msg));
}

bool create_logfile() {
	fs::create_directory("logs");
	timepoint const now = std::chrono::system_clock::now();
	logfile             = fs::absolute(fmt::format("logs/aether_{}_{}.log", date(now), timehyph(now)));
	(void)std::ofstream(logfile);
	return fs::exists(logfile);
}

} // namespace aether::_log_impl
#endif