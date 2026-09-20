#include <aether/log.hh>

#include <fstream>

#ifdef _WIN32
	#include <fcntl.h>
	#include <io.h>
	#include <windows.h>

	#include <cstdio>
#endif

namespace aether::_log_impl {

std::filesystem::path logfilepath_;

bool create_logfile_once_() {
	static bool once = false;
	if (once) {
		return true;
	}
	std::filesystem::create_directories("logs");
	auto const now = std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now());
	logfilepath_   = fmt::format("logs/aether.{:%Y-%m-%d.%H-%M-%S}.log", now);
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

#ifdef _WIN32
void attach_console_once_present_() {
	static bool once = false;
	if (once) {
		return;
	}
	if (!AttachConsole(ATTACH_PARENT_PROCESS)) {
		once = true;
		return;
	}
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	freopen_s(&fp, "CONOUT$", "w", stderr);
	freopen_s(&fp, "CONIN$", "r", stdin);
	std::ios::sync_with_stdio(true);
	once = true;
}
#endif

} // namespace aether::_log_impl