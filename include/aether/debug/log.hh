#pragma once
#ifdef AETHER_DEBUG
	#include <fmt/color.h>
	#include <fmt/format.h>
	#include <source_location>
	#include <string>
	#include <string_view>
	#include <utility>

namespace ae::log::impl {

void print(std::string_view msg, std::string_view level, fmt::color level_color, std::source_location const& loc);
void create_log_file();

} // namespace ae::log::impl

namespace ae::log {

	#ifdef AETHER_VERBOSE_DEBUG
template <typename... va>
void trace(std::source_location const& loc, fmt::format_string<va...> fmt_str, va&&... args) {
	impl::print(fmt::format(fmt_str, std::forward<va>(args)...), "TRC", fmt::color::dark_turquoise, loc);
}

template <typename... va>
void debug(std::source_location const& loc, fmt::format_string<va...> fmt_str, va&&... args) {
	impl::print(fmt::format(fmt_str, std::forward<va>(args)...), "DBG", fmt::color::medium_violet_red, loc);
}
	#endif

template <typename... va>
void info(std::source_location const& loc, fmt::format_string<va...> fmt_str, va&&... args) {
	impl::print(fmt::format(fmt_str, std::forward<va>(args)...), "INF", fmt::color::green_yellow, loc);
}

template <typename... va>
void warning(std::source_location const& loc, fmt::format_string<va...> fmt_str, va&&... args) {
	impl::print(fmt::format(fmt_str, std::forward<va>(args)...), "WRN", fmt::color::gold, loc);
}

template <typename... va>
void error(std::source_location const& loc, fmt::format_string<va...> fmt_str, va&&... args) {
	impl::print(fmt::format(fmt_str, std::forward<va>(args)...), "ERR", fmt::color::crimson, loc);
}

} // namespace ae::log

	#ifdef AETHER_VERBOSE_DEBUG
		#ifdef _MSC_VER
			#define tracelog(fmt_str, ...) ae::log::trace(std::source_location::current(), fmt_str, ##__VA_ARGS__)
			#define debuglog(fmt_str, ...) ae::log::debug(std::source_location::current(), fmt_str, ##__VA_ARGS__)
		#else
			#define tracelog(fmt_str, ...)                                                                             \
				ae::log::trace(std::source_location::current(), fmt_str __VA_OPT__(, ) __VA_ARGS__)
			#define debuglog(fmt_str, ...)                                                                             \
				ae::log::debug(std::source_location::current(), fmt_str __VA_OPT__(, ) __VA_ARGS__)
		#endif
	#endif

	#ifdef _MSC_VER
		#define infolog(fmt_str, ...)    ae::log::info(std::source_location::current(), fmt_str, ##__VA_ARGS__)
		#define warninglog(fmt_str, ...) ae::log::warning(std::source_location::current(), fmt_str, ##__VA_ARGS__)
		#define errorlog(fmt_str, ...)   ae::log::error(std::source_location::current(), fmt_str, ##__VA_ARGS__)
	#else
		#define infolog(fmt_str, ...) ae::log::info(std::source_location::current(), fmt_str __VA_OPT__(, ) __VA_ARGS__)
		#define warninglog(fmt_str, ...)                                                                               \
			ae::log::warning(std::source_location::current(), fmt_str __VA_OPT__(, ) __VA_ARGS__)
		#define errorlog(fmt_str, ...)                                                                                 \
			ae::log::error(std::source_location::current(), fmt_str __VA_OPT__(, ) __VA_ARGS__)
	#endif
#endif