#pragma once
#include <unordered_map>
#include <string>
#include <string_view>
#include <functional>
#include <cstddef>

namespace ae {

struct transparent_string_hash final {
    using is_transparent = void;

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

struct transparent_string_equal final {
    using is_transparent = void;

    bool operator()(std::string_view a, std::string_view b) const noexcept {
        return a == b;
    }
};

template <typename T>
using string_map = std::unordered_map<std::string, T, transparent_string_hash, transparent_string_equal>;

}