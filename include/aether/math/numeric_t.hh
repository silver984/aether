#pragma once
#include <concepts>
#include <type_traits>

namespace ae {

template<typename T>
concept numeric_t = std::is_arithmetic_v<T>;

}