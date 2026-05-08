#ifndef __AETHER_MATH_NUMERIC_T_HH__
#define __AETHER_MATH_NUMERIC_T_HH__

#include <concepts>
#include <type_traits>

namespace ae {

template<typename T>
concept numeric_t = std::is_arithmetic_v<T>;

}

#endif