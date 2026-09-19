#pragma once
#include <concepts>
#include <cstddef>
#include <type_traits>

namespace aether {

template <typename T>
concept numeric = std::is_arithmetic_v<T>;

template <typename T>
concept indexed_numeric = numeric<typename T::value_type> && requires(T t, T const ct, size_t i) {
	{ T::capacity() } -> std::convertible_to<size_t>;
	{ t[i] } -> std::same_as<typename T::value_type&>;
	{ ct[i] } -> std::same_as<typename T::value_type const&>;
};

} // namespace aether