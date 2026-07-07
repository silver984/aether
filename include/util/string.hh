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

struct stringhash final {
	using is_transparent = void;
	using hashtype       = std::hash<std::string_view>;

	size_t operator()(std::string_view str) const {
		return hashtype{}(str);
	}

	size_t operator()(std::string const& str) const {
		return hashtype{}(str);
	}

	size_t operator()(char const* str) const {
		return hashtype{}(str);
	}
};

template <typename T>
using stringmap = std::unordered_map<std::string, T, stringhash, std::equal_to<>>;
using stringset = std::unordered_set<std::string, stringhash, std::equal_to<>>;

} // namespace aether::util