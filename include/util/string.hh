#pragma once
#include <initializer_list>
#include <string_view>

namespace aether::util::str {

[[nodiscard]] bool string_matches_any(std::string_view str, std::initializer_list<std::string_view> list);

}