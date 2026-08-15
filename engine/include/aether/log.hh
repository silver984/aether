#pragma once
#include <aether/general.h>
#if defined(AETHER_ENGINE_HAS_DEBUG)
	#include <fmt/format.h>
	#include <source_location>
	#include <string_view>
	#include <utility>

namespace aether::_log_impl {

void print(std::string_view str, std::string_view lvl, std::source_location const& loc);
bool create_logfile();

} // namespace aether::_log_impl

namespace aether::log {

	#if !defined(AETHER_ENGINE_RELWITHDEB)
template <typename... Args>
void trace(std::source_location&& loc, fmt::format_string<Args...> fmt_str, Args&&... args) {
	_log_impl::print(fmt::format(fmt_str, std::forward<Args>(args)...), "TRACE", loc);
}

template <typename... Args>
void debug(std::source_location&& loc, fmt::format_string<Args...> fmt_str, Args&&... args) {
	_log_impl::print(fmt::format(fmt_str, std::forward<Args>(args)...), "DEBUG", loc);
}
	#endif

template <typename... Args>
void info(std::source_location&& loc, fmt::format_string<Args...> fmt_str, Args&&... args) {
	_log_impl::print(fmt::format(fmt_str, std::forward<Args>(args)...), "INFO", loc);
}

template <typename... Args>
void warn(std::source_location&& loc, fmt::format_string<Args...> fmt_str, Args&&... args) {
	_log_impl::print(fmt::format(fmt_str, std::forward<Args>(args)...), "WARN", loc);
}

template <typename... Args>
void error(std::source_location&& loc, fmt::format_string<Args...> fmt_str, Args&&... args) {
	_log_impl::print(fmt::format(fmt_str, std::forward<Args>(args)...), "ERROR", loc);
}

} // namespace aether::log

	#if !defined(AETHER_ENGINE_RELWITHDEB)
		#if defined(__clang__) || defined(__GNUC__)
			#define AETHER_ENGINE_TRACELOG(__STR__, ...)                                                                       \
				aether::log::trace(std::source_location::current(), __STR__ __VA_OPT__(, ) __VA_ARGS__)
			#define AETHER_ENGINE_DEBUGLOG(__STR__, ...)                                                                       \
				aether::log::debug(std::source_location::current(), __STR__ __VA_OPT__(, ) __VA_ARGS__)
		#else
			#define AETHER_ENGINE_TRACELOG(__STR__, ...)                                                                       \
				aether::log::trace(std::source_location::current(), __STR__, ##__VA_ARGS__)
			#define AETHER_ENGINE_DEBUGLOG(__STR__, ...)                                                                       \
				aether::log::debug(std::source_location::current(), __STR__, ##__VA_ARGS__)
		#endif
	#else
		#define AETHER_ENGINE_TRACELOG(...) ((void)0)
		#define AETHER_ENGINE_DEBUGLOG(...) ((void)0)
	#endif

	#if defined(__clang__) || defined(__GNUC__)
		#define AETHER_ENGINE_INFOLOG(__STR__, ...)                                                                                \
			aether::log::info(std::source_location::current(), __STR__ __VA_OPT__(, ) __VA_ARGS__)
		#define AETHER_ENGINE_WARNLOG(__STR__, ...)                                                                                \
			aether::log::warn(std::source_location::current(), __STR__ __VA_OPT__(, ) __VA_ARGS__)
		#define AETHER_ENGINE_ERRORLOG(__STR__, ...)                                                                               \
			aether::log::error(std::source_location::current(), __STR__ __VA_OPT__(, ) __VA_ARGS__)
	#else
		#define AETHER_ENGINE_INFOLOG(__STR__, ...)  aether::log::info(std::source_location::current(), __STR__, ##__VA_ARGS__)
		#define AETHER_ENGINE_WARNLOG(__STR__, ...)  aether::log::warn(std::source_location::current(), __STR__, ##__VA_ARGS__)
		#define AETHER_ENGINE_ERRORLOG(__STR__, ...) aether::log::error(std::source_location::current(), __STR__, ##__VA_ARGS__)
	#endif
#else
	#define AETHER_ENGINE_TRACELOG(...) ((void)0)
	#define AETHER_ENGINE_DEBUGLOG(...) ((void)0)
	#define AETHER_ENGINE_INFOLOG(...)  ((void)0)
	#define AETHER_ENGINE_WARNLOG(...)  ((void)0)
	#define AETHER_ENGINE_ERRORLOG(...) ((void)0)
#endif