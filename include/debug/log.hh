#pragma once
#ifdef AETHER_DEBUG
	#include <fmt/color.h>
	#include <fmt/format.h>
	#include <source_location>
	#include <string>
	#include <string_view>
	#include <utility>

namespace aether::log::impl_ {

void print_(std::string_view msg, std::string_view level, fmt::color level_color, std::source_location const& loc);
void create_log_file_();
void close_log_file_();

} // namespace aether::log::impl_

namespace aether::log {

	#ifdef AETHER_VERBOSE_DEBUG
template <typename... va>
void trace(std::source_location const& loc, fmt::format_string<va...> fmt_str, va&&... args) {
	impl_::print_(fmt::format(fmt_str, std::forward<va>(args)...), "TRC", fmt::color::dark_turquoise, loc);
}

template <typename... va>
void debug(std::source_location const& loc, fmt::format_string<va...> fmt_str, va&&... args) {
	impl_::print_(fmt::format(fmt_str, std::forward<va>(args)...), "DBG", fmt::color::medium_violet_red, loc);
}
	#endif

template <typename... va>
void info(std::source_location const& loc, fmt::format_string<va...> fmt_str, va&&... args) {
	impl_::print_(fmt::format(fmt_str, std::forward<va>(args)...), "INF", fmt::color::green_yellow, loc);
}

template <typename... va>
void warning(std::source_location const& loc, fmt::format_string<va...> fmt_str, va&&... args) {
	impl_::print_(fmt::format(fmt_str, std::forward<va>(args)...), "WRN", fmt::color::gold, loc);
}

template <typename... va>
void error(std::source_location const& loc, fmt::format_string<va...> fmt_str, va&&... args) {
	impl_::print_(fmt::format(fmt_str, std::forward<va>(args)...), "ERR", fmt::color::crimson, loc);
}

} // namespace aether::log

	#ifdef AETHER_VERBOSE_DEBUG
		#ifdef _MSC_VER
			#define AETHER_TRACELOG(fmt_str, ...) aether::log::trace(std::source_location::current(), fmt_str, ##__VA_ARGS__)
			#define AETHER_DEBUGLOG(fmt_str, ...) aether::log::debug(std::source_location::current(), fmt_str, ##__VA_ARGS__)
		#else
			#define AETHER_TRACELOG(fmt_str, ...)                                                                              \
				aether::log::trace(std::source_location::current(), fmt_str __VA_OPT__(, ) __VA_ARGS__)
			#define AETHER_DEBUGLOG(fmt_str, ...)                                                                              \
				aether::log::debug(std::source_location::current(), fmt_str __VA_OPT__(, ) __VA_ARGS__)
		#endif
	#else
		#define AETHER_TRACELOG(...) ((void)0)
		#define AETHER_DEBUGLOG(...) ((void)0)
	#endif

	#ifdef _MSC_VER
		#define AETHER_INFOLOG(fmt_str, ...)  aether::log::info(std::source_location::current(), fmt_str, ##__VA_ARGS__)
		#define AETHER_WARNLOG(fmt_str, ...)  aether::log::warning(std::source_location::current(), fmt_str, ##__VA_ARGS__)
		#define AETHER_ERRORLOG(fmt_str, ...) aether::log::error(std::source_location::current(), fmt_str, ##__VA_ARGS__)
	#else
		#define AETHER_INFOLOG(fmt_str, ...) aether::log::info(std::source_location::current(), fmt_str __VA_OPT__(, ) __VA_ARGS__)
		#define AETHER_WARNLOG(fmt_str, ...)                                                                                       \
			aether::log::warning(std::source_location::current(), fmt_str __VA_OPT__(, ) __VA_ARGS__)
		#define AETHER_ERRORLOG(fmt_str, ...)                                                                                      \
			aether::log::error(std::source_location::current(), fmt_str __VA_OPT__(, ) __VA_ARGS__)
	#endif
#else
	#define AETHER_TRACELOG(...) ((void)0)
	#define AETHER_DEBUGLOG(...) ((void)0)
	#define AETHER_INFOLOG(...)  ((void)0)
	#define AETHER_WARNLOG(...)  ((void)0)
	#define AETHER_ERRORLOG(...) ((void)0)
#endif