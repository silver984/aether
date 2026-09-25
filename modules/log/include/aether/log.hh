#pragma once

#include "log-impl/format_string.hh"
#include "log-impl/levels.hh"

#include <fmt/base.h>
#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <filesystem>
#include <source_location>

namespace aether::_log_impl {

constexpr std::string_view FMT_STR_           = "{:<8} | {:<5} | {}: {}\n";
constexpr std::string_view TIMESTAMP_FMT_STR_ = "{:%H:%M:%S}";

void write_to_logfile_(std::string_view str);
#ifdef _WIN32
void attach_console_once_();
#endif

} // namespace aether::_log_impl

namespace aether {

template <_log_impl::loggable_level_ T>
void log(_log_impl::format_string_ const& str, std::source_location src = std::source_location::current()) {
	std::filesystem::path const filepath(src.file_name());
	std::string const filename  = filepath.filename().string();
	std::string const where_str = fmt::format("{}:{}", filename, src.line());
	std::string_view str_view   = str.view();

	std::string_view constexpr LVL_NAME_STR = _log_impl::lvl_attr_<T>::display_name();
	auto const now                          = std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now());
	std::string const timestamp_str         = fmt::format(fmt::runtime(_log_impl::TIMESTAMP_FMT_STR_), now);
	std::string const txt = fmt::format(fmt::runtime(_log_impl::FMT_STR_), timestamp_str, LVL_NAME_STR, where_str, str_view);
	_log_impl::write_to_logfile_(txt);

	fmt::color constexpr LVL_COLOR          = _log_impl::lvl_attr_<T>::display_color();
	std::string const colored_timestamp_str = fmt::format(fmt::fg(fmt::color::gray), fmt::runtime(_log_impl::TIMESTAMP_FMT_STR_), now);
	std::string const colored_lvl_name_str  = fmt::format(fmt::fg(LVL_COLOR), LVL_NAME_STR);
#ifdef _WIN32
	_log_impl::attach_console_once_();
#endif
	fmt::print(fmt::runtime(_log_impl::FMT_STR_), colored_timestamp_str, colored_lvl_name_str, where_str, str_view);
}

} // namespace aether