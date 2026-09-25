#include <aether/log.hh>

#include <cstdint>
#include <fstream>

#ifdef _WIN32
	#include <fcntl.h>
	#include <io.h>
	#include <windows.h>

	#include <cstdio>
#endif

namespace aether::_log_impl {

std::ofstream logfile_;

enum class logfile_status_ : uint8_t {
	none,
	success,
	failed
};

logfile_status_ create_logfile_once_() {
	static logfile_status_ once = logfile_status_::none;
	if (once != logfile_status_::none) {
		return once;
	}
	std::filesystem::create_directories("logs");
	auto const now                   = std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now());
	std::filesystem::path const path = fmt::format("logs/aether.{:%Y-%m-%d.%H-%M-%S}.log", now);
	logfile_.open(path, std::ios::app);
	return once = logfile_.is_open() ? logfile_status_::success : logfile_status_::failed;
}

void write_to_logfile_(std::string_view str) {
	if (create_logfile_once_() != logfile_status_::success) {
		fmt::print(fmt::fg(fmt::color::gold), "failed to create logfile\n");
		return;
	}
	logfile_ << str;
	logfile_.flush();
}

#ifdef _WIN32
void attach_console_once_() {
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