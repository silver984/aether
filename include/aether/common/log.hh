#ifndef __AETHER_COMMON_LOG_HH__
#define __AETHER_COMMON_LOG_HH__

#ifdef AETHER_DEBUG

#include <fmt/format.h>
#include <fmt/color.h>
#include <string>
#include <string_view>
#include <source_location>
#include <utility>

namespace ae::log::impl {

void print(std::string_view msg, std::string_view level, fmt::color level_color, std::source_location const& loc);
void create_log_file();

}

namespace ae::log {

template <typename... va>
void trace(std::source_location const& loc, fmt::format_string<va...> fmt_str, va&&... args) {
	impl::print(fmt::format(fmt_str, std::forward<va>(args)...), "TRACE", fmt::color::dark_turquoise, loc);
}

template <typename... va>
void debug(std::source_location const& loc, fmt::format_string<va...> fmt_str, va&&... args) {
	impl::print(fmt::format(fmt_str, std::forward<va>(args)...), "DEBUG", fmt::color::medium_violet_red, loc);
}

template <typename... va>
void info(std::source_location const& loc, fmt::format_string<va...> fmt_str, va&&... args) {
	impl::print(fmt::format(fmt_str, std::forward<va>(args)...), "INFO", fmt::color::green_yellow, loc);
}

template <typename... va>
void warning(std::source_location const& loc, fmt::format_string<va...> fmt_str, va&&... args) {
	impl::print(fmt::format(fmt_str, std::forward<va>(args)...), "WARNING", fmt::color::gold, loc);
}

template <typename... va>
void error(std::source_location const& loc, fmt::format_string<va...> fmt_str, va&&... args) {
	impl::print(fmt::format(fmt_str, std::forward<va>(args)...), "ERROR", fmt::color::crimson, loc);
}

}

#define tracelog(fmt_str, ...)\
	ae::log::trace(std::source_location::current(), fmt_str, ##__VA_ARGS__)

#define debuglog(fmt_str, ...)\
	ae::log::debug(std::source_location::current(), fmt_str, ##__VA_ARGS__)

#define infolog(fmt_str, ...)\
	ae::log::info(std::source_location::current(), fmt_str, ##__VA_ARGS__)

#define warninglog(fmt_str, ...)\
	ae::log::warning(std::source_location::current(), fmt_str, ##__VA_ARGS__)

#define errorlog(fmt_str, ...)\
	ae::log::debug(std::source_location::current(), fmt_str, ##__VA_ARGS__)

#else

#define tracelog(...) ((void)0)
#define debuglog(...) ((void)0)
#define infolog(...) ((void)0)
#define warninglog(...) ((void)0)
#define errorlog(...) ((void)0)

#endif

#endif