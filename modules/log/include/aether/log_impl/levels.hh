#pragma once

#include <fmt/color.h>

#include <concepts>
#include <string_view>
#include <type_traits>

namespace aether::_log_impl {

struct lvl_ {
	lvl_() = delete;
};

struct info_lvl_ final : lvl_ {};
struct warn_lvl_ final : lvl_ {};
struct err_lvl_ final : lvl_ {};
struct deb_lvl_ final : lvl_ {};
struct trace_lvl_ final : lvl_ {};

template <typename T_>
        requires std::is_base_of_v<lvl_, T_>
struct lvl_attr_;

template <>
struct lvl_attr_<info_lvl_> final {
	lvl_attr_() = delete;
	[[nodiscard]] static constexpr std::string_view display_name() noexcept { return "info"; }
	[[nodiscard]] static constexpr fmt::color display_color() noexcept { return fmt::color::green_yellow; }
};

template <>
struct lvl_attr_<warn_lvl_> final {
	lvl_attr_() = delete;
	[[nodiscard]] static constexpr std::string_view display_name() noexcept { return "warn"; }
	[[nodiscard]] static constexpr fmt::color display_color() noexcept { return fmt::color::gold; }
};

template <>
struct lvl_attr_<err_lvl_> final {
	lvl_attr_() = delete;
	[[nodiscard]] static constexpr std::string_view display_name() noexcept { return "error"; }
	[[nodiscard]] static constexpr fmt::color display_color() noexcept { return fmt::color::red; }
};

template <>
struct lvl_attr_<deb_lvl_> final {
	lvl_attr_() = delete;
	[[nodiscard]] static constexpr std::string_view display_name() noexcept { return "debug"; }
	[[nodiscard]] static constexpr fmt::color display_color() noexcept { return fmt::color::purple; }
};

template <>
struct lvl_attr_<trace_lvl_> final {
	lvl_attr_() = delete;
	[[nodiscard]] static constexpr std::string_view display_name() noexcept { return "trace"; }
	[[nodiscard]] static constexpr fmt::color display_color() noexcept { return fmt::color::blue; }
};

template <typename T_>
concept loggable_level_ = requires {
	typename lvl_attr_<T_>;
	{ lvl_attr_<T_>::display_name() } -> std::same_as<std::string_view>;
	{ lvl_attr_<T_>::display_color() } -> std::same_as<fmt::color>;
};

} // namespace aether::_log_impl

namespace aether {

using info  = _log_impl::info_lvl_;
using warn  = _log_impl::warn_lvl_;
using error = _log_impl::err_lvl_;
using debug = _log_impl::deb_lvl_;
using trace = _log_impl::trace_lvl_;

} // namespace aether