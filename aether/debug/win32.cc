#include <debug/win32.hh>
#include <windows.h>

namespace aether::win32_ {

bool enable_console_colors_() {
	HANDLE h_out  = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dw_mode = 0;
	GetConsoleMode(h_out, &dw_mode);
	dw_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(h_out, dw_mode);
	return true;
}

} // namespace aether::win32_