#pragma once
#include <cstddef>
#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>

namespace aether::util {

struct transparent_string_hash final {
	using is_transparent = void;

	std::size_t operator()(std::string_view s) const noexcept {
		return std::hash<std::string_view>{}(s);
	}

	std::size_t operator()(std::string const& s) const noexcept {
		return std::hash<std::string_view>{}(s);
	}

	std::size_t operator()(char const* s) const noexcept {
		return std::hash<std::string_view>{}(s);
	}
};

struct transparent_string_equal final {
	using is_transparent = void;

	bool operator()(std::string_view a, std::string_view b) const noexcept {
		return a == b;
	}
};

template <typename T>
using string_map = std::unordered_map<std::string, T, transparent_string_hash, transparent_string_equal>;

} // namespace aether::util