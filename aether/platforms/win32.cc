#include <platforms/win32.hh>
#include <windows.h>

namespace aether::win32 {

bool enable_console_colors() {
	HANDLE h_out = GetStdHandle(STD_OUTPUT_HANDLE);

	if (h_out == INVALID_HANDLE_VALUE) {
		return false;
	}

	DWORD dw_mode = 0;

	if (!GetConsoleMode(h_out, &dw_mode)) {
		return false;
	}

	dw_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

	SetConsoleMode(h_out, dw_mode);

	return true;
}

} // namespace aether::win32