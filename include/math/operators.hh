#pragma once
#include <cstddef>
#include <functional>
#include <math/numeric.hh>
#include <type_traits>

namespace aether {

template <template <typename> class derived, numeric T, std::size_t N>
struct operators {
	[[nodiscard]] static constexpr std::size_t capacity() {
		return N;
	}

	[[nodiscard]] constexpr derived<T> operator+() const {
		return self();
	}

	[[nodiscard]] constexpr derived<T> operator-() const {
		derived<T> out;
		for (std::size_t i = 0; i < N; ++i) {
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

	constexpr derived<T>& operator+=(derived<T> const& rhs) {
		for (std::size_t i = 0; i < N; ++i) {
			self()[i] += rhs[i];
		}
		return self();
	}

	template <numeric U>
	constexpr derived<T>& operator+=(derived<U> const& rhs) {
		for (std::size_t i = 0; i < N; ++i) {
			self()[i] += static_cast<T>(rhs[i]);
		}
		return self();
	}

	template <numeric U>
	constexpr derived<T>& operator+=(U rhs) {
		for (std::size_t i = 0; i < N; ++i) {
			self()[i] += static_cast<T>(rhs);
		}
		return self();
	}

	constexpr derived<T>& operator-=(derived<T> const& rhs) {
		for (std::size_t i = 0; i < N; ++i) {
			self()[i] -= rhs[i];
		}
		return self();
	}

	template <numeric U>
	constexpr derived<T>& operator-=(derived<U> const& rhs) {
		for (std::size_t i = 0; i < N; ++i) {
			self()[i] -= static_cast<T>(rhs[i]);
		}
		return self();
	}

	template <numeric U>
	constexpr derived<T>& operator-=(U rhs) {
		for (std::size_t i = 0; i < N; ++i) {
			self()[i] -= static_cast<T>(rhs);
		}
		return self();
	}

	constexpr derived<T>& operator*=(derived<T> const& rhs) {
		for (std::size_t i = 0; i < N; ++i) {
			self()[i] *= rhs[i];
		}
		return self();
	}

	template <numeric U>
	constexpr derived<T>& operator*=(derived<U> const& rhs) {
		for (std::size_t i = 0; i < N; ++i) {
			self()[i] *= static_cast<T>(rhs[i]);
		}
		return self();
	}

	template <numeric U>
	constexpr derived<T>& operator*=(U rhs) {
		for (std::size_t i = 0; i < N; ++i) {
			self()[i] *= static_cast<T>(rhs);
		}
		return self();
	}

	constexpr derived<T>& operator/=(derived<T> const& rhs) {
		for (std::size_t i = 0; i < N; ++i) {
			self()[i] /= rhs[i];
		}
		return self();
	}

	template <numeric U>
	constexpr derived<T>& operator/=(derived<U> const& rhs) {
		for (std::size_t i = 0; i < N; ++i) {
			self()[i] /= static_cast<T>(rhs[i]);
		}
		return self();
	}

	template <numeric U>
	constexpr derived<T>& operator/=(U rhs) {
		for (std::size_t i = 0; i < N; ++i) {
			self()[i] /= static_cast<T>(rhs);
		}
		return self();
	}

	constexpr derived<T>& operator++() {
		for (std::size_t i = 0; i < N; ++i) {
			++self()[i];
		}
		return self();
	}

	constexpr derived<T> operator++(int) {
		derived<T> out(self());
		++self();
		return out;
	}

	constexpr derived<T>& operator--() {
		for (std::size_t i = 0; i < N; ++i) {
			--self()[i];
		}
		return self();
	}

	constexpr derived<T> operator--(int) {
		derived<T> out(self());
		--self();
		return out;
	}

	[[nodiscard]] constexpr bool operator==(derived<T> const& rhs) const {
		for (std::size_t i = 0; i < N; ++i) {
			if (self()[i] != rhs[i]) {
				return false;
			}
		}
		return true;
	}

	[[nodiscard]] constexpr bool operator!=(derived<T> const& rhs) const {
		return !(self() == rhs);
	}

	template <numeric U>
	[[nodiscard]] explicit constexpr operator derived<U>() const {
		derived<U> out;
		for (std::size_t i = 0; i < N; ++i) {
			out[i] = static_cast<U>(self()[i]);
		}
		return out;
	}

private:
	template <indexed_numeric Rhs, typename F>
	constexpr auto binary(Rhs const& rhs, F&& op) const -> derived<std::common_type_t<T, typename Rhs::value_type>> {
		derived<std::common_type_t<T, typename Rhs::value_type>> out;
		for (std::size_t i = 0; i < N; ++i) {
			out[i] = op(self()[i], rhs[i]);
		}
		return out;
	}

	template <numeric U, typename F>
	constexpr auto binary(U rhs, F&& op) const -> derived<std::common_type_t<T, U>> {
		derived<std::common_type_t<T, U>> out;
		for (std::size_t i = 0; i < N; ++i) {
			out[i] = op(self()[i], rhs);
		}
		return out;
	}

	[[nodiscard]] constexpr derived<T> const& self() const {
		return static_cast<derived<T> const&>(*this);
	}

	[[nodiscard]] constexpr derived<T>& self() {
		return static_cast<derived<T>&>(*this);
	}
};

} // namespace aether