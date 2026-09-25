#pragma once

#include <fmt/format.h>

#include <string>
#include <string_view>
#include <utility>

namespace aether::_log_impl {

class format_string_ final {
public:
	template <typename... T>
	format_string_(fmt::format_string<T...> str, T&&... args)
	        : str_(fmt::format(str, std::forward<T>(args)...)) {}
	[[nodiscard]] std::string_view view() const { return str_; }

private:
	std::string str_;
};

} // namespace aether::_log_impl