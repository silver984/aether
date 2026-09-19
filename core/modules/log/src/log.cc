#include <aether/log.hh>

#include <fstream>

namespace aether::_log_impl {

std::filesystem::path logfilepath_;

bool create_logfile_once_() {
	static bool once = false;
	if (once) { return true; }
	std::filesystem::create_directories("logs");
	auto const now = std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now());
	logfilepath_   = fmt::format("logs/aether_{:%Y-%m-%d_%H-%M-%S}.log", now);
	std::ofstream const logfile(logfilepath_, std::ios::app);
	return once = logfile.is_open();
}

void write_to_logfile_(std::string_view str) {
	if (!create_logfile_once_()) {
		fmt::print(fmt::fg(fmt::color::gold), "failed to create logfile\n");
		return;
	}
	std::ofstream logfile(logfilepath_, std::ios::app);
	if (!logfile.is_open()) {
		fmt::print(fmt::fg(fmt::color::gold), "failed to open logfile\n");
		return;
	}
	logfile << str;
}

} // namespace aether::_log_impl