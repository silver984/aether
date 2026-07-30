#include <debug/log.hh>
#if defined(AE_HAS_DEBUG)
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

std::ofstream logfile;

} // namespace

namespace aether::log_impl_ {

void print_(std::string_view msg, std::string_view lvl, std::source_location const& loc) {
	timepoint const now      = std::chrono::system_clock::now();
	std::string const logmsg = fmt::format("{:<8} | {:<5} | {:<14} | {}", time(now), lvl, where(loc), msg);
	if (logfile.is_open()) {
		logfile << logmsg << "\n";
	}
	fmt::println(fmt::runtime(logmsg));
}

bool create_logfile_() {
	fs::create_directory("logs");
	timepoint const now     = std::chrono::system_clock::now();
	fs::path const filepath = fmt::format("logs/aether_{}_{}.log", date(now), timehyph(now));
	logfile.open(filepath);
	return logfile.is_open();
}

void close_logfile_() {
	if (logfile.is_open()) {
		logfile.close();
	}
}

} // namespace aether::log_impl_
#endif