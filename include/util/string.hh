#pragma once
#include <cstddef>
#include <functional>
#include <initializer_list>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>

namespace aether::util {

[[nodiscard]] bool string_matches_any(std::string_view str, std::initializer_list<std::string_view> list);

struct string_hash final {
	using is_transparent = void;
	using hash_type      = std::hash<std::string_view>;

	size_t operator()(std::string_view str) const {
		return hash_type{}(str);
	}

	size_t operator()(std::string const& str) const {
		return hash_type{}(str);
	}

	size_t operator()(char const* str) const {
		return hash_type{}(str);
	}
};

template <typename T>
using string_map = std::unordered_map<std::string, T, string_hash, std::equal_to<>>;
using string_set = std::unordered_set<std::string, string_hash, std::equal_to<>>;

} // namespace aether::util