#pragma once
#include <concepts>
#include <type_traits>

namespace ae {

template <typename T>
concept numeric_type = std::is_arithmetic_v<T>;

}