#pragma once
#ifdef AETHER_DEBUG
	#include <fmt/color.h>
	#include <fmt/format.h>
	#include <source_location>
	#include <string>
	#include <string_view>
	#include <utility>

namespace aether::log::__impl {

void __print(std::string_view msg, std::string_view level, fmt::color level_color, std::source_location const& loc);
void __create_log_file();

} // namespace aether::log::__impl

namespace aether::log {

	#ifdef AETHER_VERBOSE_DEBUG
template <typename... Args>
void trace(std::source_location const& loc, fmt::format_string<Args...> fmt_str, Args&&... args) {
	__impl::__print(fmt::format(fmt_str, std::forward<Args>(args)...), "TRC", fmt::color::dark_turquoise, loc);
}

template <typename... Args>
void debug(std::source_location const& loc, fmt::format_string<Args...> fmt_str, Args&&... args) {
	__impl::__print(fmt::format(fmt_str, std::forward<Args>(args)...), "DBG", fmt::color::medium_violet_red, loc);
}
	#endif

template <typename... Args>
void info(std::source_location const& loc, fmt::format_string<Args...> fmt_str, Args&&... args) {
	__impl::__print(fmt::format(fmt_str, std::forward<Args>(args)...), "INF", fmt::color::green_yellow, loc);
}

template <typename... Args>
void warning(std::source_location const& loc, fmt::format_string<Args...> fmt_str, Args&&... args) {
	__impl::__print(fmt::format(fmt_str, std::forward<Args>(args)...), "WRN", fmt::color::gold, loc);
}

template <typename... Args>
void error(std::source_location const& loc, fmt::format_string<Args...> fmt_str, Args&&... args) {
	__impl::__print(fmt::format(fmt_str, std::forward<Args>(args)...), "ERR", fmt::color::crimson, loc);
}

} // namespace aether::log

	#ifdef AETHER_VERBOSE_DEBUG
		#ifdef _MSC_VER
			#define tracelog(fmt_str, ...) aether::log::trace(std::source_location::current(), fmt_str, ##__VA_ARGS__)
			#define debuglog(fmt_str, ...) aether::log::debug(std::source_location::current(), fmt_str, ##__VA_ARGS__)
		#else
			#define tracelog(fmt_str, ...)                                                                             \
				aether::log::trace(std::source_location::current(), fmt_str __VA_OPT__(, ) __VA_ARGS__)
			#define debuglog(fmt_str, ...)                                                                             \
				aether::log::debug(std::source_location::current(), fmt_str __VA_OPT__(, ) __VA_ARGS__)
		#endif
	#endif

	#ifdef _MSC_VER
		#define infolog(fmt_str, ...)    aether::log::info(std::source_location::current(), fmt_str, ##__VA_ARGS__)
		#define warninglog(fmt_str, ...) aether::log::warning(std::source_location::current(), fmt_str, ##__VA_ARGS__)
		#define errorlog(fmt_str, ...)   aether::log::error(std::source_location::current(), fmt_str, ##__VA_ARGS__)
	#else
		#define infolog(fmt_str, ...)                                                                                  \
			aether::log::info(std::source_location::current(), fmt_str __VA_OPT__(, ) __VA_ARGS__)
		#define warninglog(fmt_str, ...)                                                                               \
			aether::log::warning(std::source_location::current(), fmt_str __VA_OPT__(, ) __VA_ARGS__)
		#define errorlog(fmt_str, ...)                                                                                 \
			aether::log::error(std::source_location::current(), fmt_str __VA_OPT__(, ) __VA_ARGS__)
	#endif
#endif