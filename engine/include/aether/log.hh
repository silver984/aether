#pragma once
#include <aether/general.h>

#if defined(__ae_anydebug__)
	#include <fmt/format.h>

	#include <source_location>
	#include <string_view>
	#include <utility>

namespace aether::_log_impl {

void print_(std::string_view str, std::string_view lvl, std::source_location const& loc);
bool create_logfile_();

} // namespace aether::_log_impl

namespace aether::log {

	#if !defined(__ae_relwithdeb__)
template <typename... Args>
void trace(std::source_location&& loc, fmt::format_string<Args...> fmt_str, Args&&... args) {
	_log_impl::print_(fmt::format(fmt_str, std::forward<Args>(args)...), "TRACE", loc);
}

template <typename... Args>
void debug(std::source_location&& loc, fmt::format_string<Args...> fmt_str, Args&&... args) {
	_log_impl::print_(fmt::format(fmt_str, std::forward<Args>(args)...), "DEBUG", loc);
}
	#endif

template <typename... Args>
void info(std::source_location&& loc, fmt::format_string<Args...> fmt_str, Args&&... args) {
	_log_impl::print_(fmt::format(fmt_str, std::forward<Args>(args)...), "INFO", loc);
}

template <typename... Args>
void warn(std::source_location&& loc, fmt::format_string<Args...> fmt_str, Args&&... args) {
	_log_impl::print_(fmt::format(fmt_str, std::forward<Args>(args)...), "WARN", loc);
}

template <typename... Args>
void error(std::source_location&& loc, fmt::format_string<Args...> fmt_str, Args&&... args) {
	_log_impl::print_(fmt::format(fmt_str, std::forward<Args>(args)...), "ERROR", loc);
}

} // namespace aether::log

	#if !defined(__ae_relwithdeb__)
		#if defined(__clang__) || defined(__GNUC__)
			#define ae_trace(__STR__, ...)                                                                                     \
				aether::log::trace(std::source_location::current(), __STR__ __VA_OPT__(, ) __VA_ARGS__)
			#define ae_debug(__STR__, ...)                                                                                     \
				aether::log::debug(std::source_location::current(), __STR__ __VA_OPT__(, ) __VA_ARGS__)
		#else
			#define ae_trace(__STR__, ...) aether::log::trace(std::source_location::current(), __STR__, ##__VA_ARGS__)
			#define ae_debug(__STR__, ...) aether::log::debug(std::source_location::current(), __STR__, ##__VA_ARGS__)
		#endif
	#else
		#define ae_trace(...) ((void)0)
		#define ae_debug(...) ((void)0)
	#endif

	#if defined(__clang__) || defined(__GNUC__)
		#define ae_info(__STR__, ...)  aether::log::info(std::source_location::current(), __STR__ __VA_OPT__(, ) __VA_ARGS__)
		#define ae_warn(__STR__, ...)  aether::log::warn(std::source_location::current(), __STR__ __VA_OPT__(, ) __VA_ARGS__)
		#define ae_error(__STR__, ...) aether::log::error(std::source_location::current(), __STR__ __VA_OPT__(, ) __VA_ARGS__)
	#else
		#define ae_info(__STR__, ...)  aether::log::info(std::source_location::current(), __STR__, ##__VA_ARGS__)
		#define ae_warn(__STR__, ...)  aether::log::warn(std::source_location::current(), __STR__, ##__VA_ARGS__)
		#define ae_error(__STR__, ...) aether::log::error(std::source_location::current(), __STR__, ##__VA_ARGS__)
	#endif
#else
	#define ae_trace(...) ((void)0)
	#define ae_debug(...) ((void)0)
	#define ae_info(...)  ((void)0)
	#define ae_warn(...)  ((void)0)
	#define ae_error(...) ((void)0)
#endif