#pragma once
#include <config.h>
#if defined(AETHER_ENGINE_HAS_DEBUG)
	#include <fmt/format.h>
	#include <source_location>
	#include <string_view>
	#include <utility>

namespace aether::log_impl_ {

void print_(std::string_view str, std::string_view lvl, std::source_location const& loc);
bool create_logfile_();
void close_logfile_();

} // namespace aether::log_impl_

namespace aether::log {

	#if !defined(AETHER_ENGINE_RELWITHDEB)
template <typename... va>
void trace(std::source_location const& loc, fmt::format_string<va...> fmtstr, va&&... args) {
	log_impl_::print_(fmt::format(fmtstr, std::forward<va>(args)...), "TRACE", loc);
}

template <typename... va>
void debug(std::source_location const& loc, fmt::format_string<va...> fmtstr, va&&... args) {
	log_impl_::print_(fmt::format(fmtstr, std::forward<va>(args)...), "DEBUG", loc);
}
	#endif

template <typename... va>
void info(std::source_location const& loc, fmt::format_string<va...> fmtstr, va&&... args) {
	log_impl_::print_(fmt::format(fmtstr, std::forward<va>(args)...), "INFO", loc);
}

template <typename... va>
void warn(std::source_location const& loc, fmt::format_string<va...> fmtstr, va&&... args) {
	log_impl_::print_(fmt::format(fmtstr, std::forward<va>(args)...), "WARN", loc);
}

template <typename... va>
void error(std::source_location const& loc, fmt::format_string<va...> fmtstr, va&&... args) {
	log_impl_::print_(fmt::format(fmtstr, std::forward<va>(args)...), "ERROR", loc);
}

} // namespace aether::log

	#if !defined(AETHER_ENGINE_RELWITHDEB)
		#if defined(_MSC_VER)
			#define AETHER_TRACELOG(__STR__, ...) aether::log::trace(std::source_location::current(), __STR__, ##__VA_ARGS__)
			#define AETHER_DEBUGLOG(__STR__, ...) aether::log::debug(std::source_location::current(), __STR__, ##__VA_ARGS__)
		#else
			#define AETHER_TRACELOG(__STR__, ...)                                                                              \
				aether::log::trace(std::source_location::current(), __STR__ __VA_OPT__(, ) __VA_ARGS__)
			#define AETHER_DEBUGLOG(__STR__, ...)                                                                              \
				aether::log::debug(std::source_location::current(), __STR__ __VA_OPT__(, ) __VA_ARGS__)
		#endif
	#else
		#define AETHER_TRACELOG(...) ((void)0)
		#define AETHER_DEBUGLOG(...) ((void)0)
	#endif

	#if defined(_MSC_VER)
		#define AETHER_INFOLOG(__STR__, ...)  aether::log::info(std::source_location::current(), __STR__, ##__VA_ARGS__)
		#define AETHER_WARNLOG(__STR__, ...)  aether::log::warn(std::source_location::current(), __STR__, ##__VA_ARGS__)
		#define AETHER_ERRORLOG(__STR__, ...) aether::log::error(std::source_location::current(), __STR__, ##__VA_ARGS__)
	#else
		#define AETHER_INFOLOG(__STR__, ...) aether::log::info(std::source_location::current(), __STR__ __VA_OPT__(, ) __VA_ARGS__)
		#define AETHER_WARNLOG(__STR__, ...) aether::log::warn(std::source_location::current(), __STR__ __VA_OPT__(, ) __VA_ARGS__)
		#define AETHER_ERRORLOG(__STR__, ...)                                                                                      \
			aether::log::error(std::source_location::current(), __STR__ __VA_OPT__(, ) __VA_ARGS__)
	#endif
#else
	#define AETHER_TRACELOG(...) ((void)0)
	#define AETHER_DEBUGLOG(...) ((void)0)
	#define AETHER_INFOLOG(...)  ((void)0)
	#define AETHER_WARNLOG(...)  ((void)0)
	#define AETHER_ERRORLOG(...) ((void)0)
#endif