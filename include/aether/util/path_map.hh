#pragma once
#include <cstddef>
#include <filesystem>
#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>

namespace ae {

struct transparent_path_hash final {
	using is_transparent = void;

	size_t operator()(std::filesystem::path const& p) const noexcept {
		return std::hash<std::string_view>{}(p.string());
	}

	size_t operator()(std::string_view s) const noexcept {
		return std::hash<std::string_view>{}(s);
	}

	size_t operator()(std::string const& s) const noexcept {
		return std::hash<std::string_view>{}(s);
	}

	size_t operator()(char const* s) const noexcept {
		return std::hash<std::string_view>{}(s);
	}
};

struct transparent_path_equal final {
	using is_transparent = void;

	bool operator()(std::filesystem::path const& a, std::filesystem::path const& b) const noexcept {
		return a == b;
	}

	bool operator()(std::filesystem::path const& a, std::string_view b) const noexcept {
		return a.string() == b;
	}

	bool operator()(std::string_view a, std::filesystem::path const& b) const noexcept {
		return a == b.string();
	}
};

template <typename T>
using path_map = std::unordered_map<std::filesystem::path, T, transparent_path_hash, transparent_path_equal>;

} // namespace ae