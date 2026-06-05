#pragma once
#include <cstddef>
#include <functional>
#include <math/numeric.hh>
#include <type_traits>

namespace aether {

template <template <typename> class Derived, numeric T, size_t Capacity>
struct operators {
	[[nodiscard]] static constexpr size_t capacity() {
		return Capacity;
	}

	[[nodiscard]] constexpr Derived<T> operator+() const {
		return self();
	}

	[[nodiscard]] constexpr Derived<T> operator-() const {
		Derived<T> out;
		for (size_t i = 0; i < Capacity; ++i) {
			out[i] = -(self()[i]);
		}
		return out;
	}

	template <indexed_numeric Rhs>
	[[nodiscard]] constexpr auto operator+(Rhs const& rhs) const {
		return binary(rhs, std::plus<>{});
	}

	template <numeric U>
	[[nodiscard]] constexpr auto operator+(U rhs) const {
		return binary(rhs, std::plus<>{});
	}

	template <indexed_numeric Rhs>
	[[nodiscard]] constexpr auto operator-(Rhs const& rhs) const {
		return binary(rhs, std::minus<>{});
	}

	template <numeric U>
	[[nodiscard]] constexpr auto operator-(U rhs) const {
		return binary(rhs, std::minus<>{});
	}

	template <indexed_numeric Rhs>
	[[nodiscard]] constexpr auto operator*(Rhs const& rhs) const {
		return binary(rhs, std::multiplies<>{});
	}

	template <numeric U>
	[[nodiscard]] constexpr auto operator*(U rhs) const {
		return binary(rhs, std::multiplies<>{});
	}

	template <indexed_numeric Rhs>
	[[nodiscard]] constexpr auto operator/(Rhs const& rhs) const {
		return binary(rhs, std::divides<>{});
	}

	template <numeric U>
	[[nodiscard]] constexpr auto operator/(U rhs) const {
		return binary(rhs, std::divides<>{});
	}

	constexpr Derived<T>& operator+=(Derived<T> const& rhs) {
		for (size_t i = 0; i < Capacity; ++i) {
			self()[i] += rhs[i];
		}
		return self();
	}

	template <numeric U>
	constexpr Derived<T>& operator+=(Derived<U> const& rhs) {
		for (size_t i = 0; i < Capacity; ++i) {
			self()[i] += static_cast<T>(rhs[i]);
		}
		return self();
	}

	template <numeric U>
	constexpr Derived<T>& operator+=(U rhs) {
		for (size_t i = 0; i < Capacity; ++i) {
			self()[i] += static_cast<T>(rhs);
		}
		return self();
	}

	constexpr Derived<T>& operator-=(Derived<T> const& rhs) {
		for (size_t i = 0; i < Capacity; ++i) {
			self()[i] -= rhs[i];
		}
		return self();
	}

	template <numeric U>
	constexpr Derived<T>& operator-=(Derived<U> const& rhs) {
		for (size_t i = 0; i < Capacity; ++i) {
			self()[i] -= static_cast<T>(rhs[i]);
		}
		return self();
	}

	template <numeric U>
	constexpr Derived<T>& operator-=(U rhs) {
		for (size_t i = 0; i < Capacity; ++i) {
			self()[i] -= static_cast<T>(rhs);
		}
		return self();
	}

	constexpr Derived<T>& operator*=(Derived<T> const& rhs) {
		for (size_t i = 0; i < Capacity; ++i) {
			self()[i] *= rhs[i];
		}
		return self();
	}

	template <numeric U>
	constexpr Derived<T>& operator*=(Derived<U> const& rhs) {
		for (size_t i = 0; i < Capacity; ++i) {
			self()[i] *= static_cast<T>(rhs[i]);
		}
		return self();
	}

	template <numeric U>
	constexpr Derived<T>& operator*=(U rhs) {
		for (size_t i = 0; i < Capacity; ++i) {
			self()[i] *= static_cast<T>(rhs);
		}
		return self();
	}

	constexpr Derived<T>& operator/=(Derived<T> const& rhs) {
		for (size_t i = 0; i < Capacity; ++i) {
			self()[i] /= rhs[i];
		}
		return self();
	}

	template <numeric U>
	constexpr Derived<T>& operator/=(Derived<U> const& rhs) {
		for (size_t i = 0; i < Capacity; ++i) {
			self()[i] /= static_cast<T>(rhs[i]);
		}
		return self();
	}

	template <numeric U>
	constexpr Derived<T>& operator/=(U rhs) {
		for (size_t i = 0; i < Capacity; ++i) {
			self()[i] /= static_cast<T>(rhs);
		}
		return self();
	}

	constexpr Derived<T>& operator++() {
		for (size_t i = 0; i < Capacity; ++i) {
			++self()[i];
		}
		return self();
	}

	constexpr Derived<T> operator++(int) {
		Derived<T> out(self());
		++self();
		return out;
	}

	constexpr Derived<T>& operator--() {
		for (size_t i = 0; i < Capacity; ++i) {
			--self()[i];
		}
		return self();
	}

	constexpr Derived<T> operator--(int) {
		Derived<T> out(self());
		--self();
		return out;
	}

	[[nodiscard]] constexpr bool operator==(Derived<T> const& rhs) const {
		for (size_t i = 0; i < Capacity; ++i) {
			if (self()[i] != rhs[i]) {
				return false;
			}
		}
		return true;
	}

	[[nodiscard]] constexpr bool operator!=(Derived<T> const& rhs) const {
		return !(self() == rhs);
	}

	template <numeric U>
	[[nodiscard]] explicit constexpr operator Derived<U>() const {
		Derived<U> out;
		for (size_t i = 0; i < Capacity; ++i) {
			out[i] = static_cast<U>(self()[i]);
		}
		return out;
	}

private:
	template <indexed_numeric Rhs, typename F>
	constexpr auto binary(Rhs const& rhs, F&& op) const -> Derived<std::common_type_t<T, typename Rhs::value_type>> {
		Derived<std::common_type_t<T, typename Rhs::value_type>> out;
		for (size_t i = 0; i < Capacity; ++i) {
			out[i] = op(self()[i], rhs[i]);
		}
		return out;
	}

	template <numeric U, typename F>
	constexpr auto binary(U rhs, F&& op) const -> Derived<std::common_type_t<T, U>> {
		Derived<std::common_type_t<T, U>> out;
		for (size_t i = 0; i < Capacity; ++i) {
			out[i] = op(self()[i], rhs);
		}
		return out;
	}

	[[nodiscard]] constexpr Derived<T> const& self() const {
		return static_cast<Derived<T> const&>(*this);
	}

	[[nodiscard]] constexpr Derived<T>& self() {
		return static_cast<Derived<T>&>(*this);
	}
};

} // namespace aether