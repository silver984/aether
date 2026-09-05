#pragma once
#include <aether/numeric.hh>

#include <cstddef>
#include <functional>
#include <type_traits>

namespace aether {

template <template <typename> class Derived, numeric T, size_t N>
struct operators {
	[[nodiscard]] static constexpr size_t capacity() { return N; }

	[[nodiscard]] constexpr Derived<T> operator+() const { return self_(); }

	[[nodiscard]] constexpr Derived<T> operator-() const {
		Derived<T> out;
		for (size_t i = 0; i < N; ++i) {
			out[i] = -(self_()[i]);
		}
		return out;
	}

	template <indexed_numeric Other>
	[[nodiscard]] constexpr auto operator+(Other const& other) const {
		return binary_(other, std::plus<>{});
	}

	template <numeric U>
	[[nodiscard]] constexpr auto operator+(U other) const {
		return binary_(other, std::plus<>{});
	}

	template <indexed_numeric Other>
	[[nodiscard]] constexpr auto operator-(Other const& other) const {
		return binary_(other, std::minus<>{});
	}

	template <numeric U>
	[[nodiscard]] constexpr auto operator-(U other) const {
		return binary_(other, std::minus<>{});
	}

	template <indexed_numeric Other>
	[[nodiscard]] constexpr auto operator*(Other const& other) const {
		return binary_(other, std::multiplies<>{});
	}

	template <numeric U>
	[[nodiscard]] constexpr auto operator*(U other) const {
		return binary_(other, std::multiplies<>{});
	}

	template <indexed_numeric Other>
	[[nodiscard]] constexpr auto operator/(Other const& other) const {
		return binary_(other, std::divides<>{});
	}

	template <numeric U>
	[[nodiscard]] constexpr auto operator/(U other) const {
		return binary_(other, std::divides<>{});
	}

	constexpr Derived<T>& operator+=(Derived<T> const& other) {
		for (size_t i = 0; i < N; ++i) {
			self_()[i] += other[i];
		}
		return self_();
	}

	template <numeric U>
	constexpr Derived<T>& operator+=(Derived<U> const& other) {
		for (size_t i = 0; i < N; ++i) {
			self_()[i] += static_cast<T>(other[i]);
		}
		return self_();
	}

	template <numeric U>
	constexpr Derived<T>& operator+=(U other) {
		for (size_t i = 0; i < N; ++i) {
			self_()[i] += static_cast<T>(other);
		}
		return self_();
	}

	constexpr Derived<T>& operator-=(Derived<T> const& other) {
		for (size_t i = 0; i < N; ++i) {
			self_()[i] -= other[i];
		}
		return self_();
	}

	template <numeric U>
	constexpr Derived<T>& operator-=(Derived<U> const& other) {
		for (size_t i = 0; i < N; ++i) {
			self_()[i] -= static_cast<T>(other[i]);
		}
		return self_();
	}

	template <numeric U>
	constexpr Derived<T>& operator-=(U other) {
		for (size_t i = 0; i < N; ++i) {
			self_()[i] -= static_cast<T>(other);
		}
		return self_();
	}

	constexpr Derived<T>& operator*=(Derived<T> const& other) {
		for (size_t i = 0; i < N; ++i) {
			self_()[i] *= other[i];
		}
		return self_();
	}

	template <numeric U>
	constexpr Derived<T>& operator*=(Derived<U> const& other) {
		for (size_t i = 0; i < N; ++i) {
			self_()[i] *= static_cast<T>(other[i]);
		}
		return self_();
	}

	template <numeric U>
	constexpr Derived<T>& operator*=(U other) {
		for (size_t i = 0; i < N; ++i) {
			self_()[i] *= static_cast<T>(other);
		}
		return self_();
	}

	constexpr Derived<T>& operator/=(Derived<T> const& other) {
		for (size_t i = 0; i < N; ++i) {
			self_()[i] /= other[i];
		}
		return self_();
	}

	template <numeric U>
	constexpr Derived<T>& operator/=(Derived<U> const& other) {
		for (size_t i = 0; i < N; ++i) {
			self_()[i] /= static_cast<T>(other[i]);
		}
		return self_();
	}

	template <numeric U>
	constexpr Derived<T>& operator/=(U other) {
		for (size_t i = 0; i < N; ++i) {
			self_()[i] /= static_cast<T>(other);
		}
		return self_();
	}

	constexpr Derived<T>& operator++() {
		for (size_t i = 0; i < N; ++i) {
			++self_()[i];
		}
		return self_();
	}

	constexpr Derived<T> operator++(int) {
		Derived<T> out(self_());
		++self_();
		return out;
	}

	constexpr Derived<T>& operator--() {
		for (size_t i = 0; i < N; ++i) {
			--self_()[i];
		}
		return self_();
	}

	constexpr Derived<T> operator--(int) {
		Derived<T> out(self_());
		--self_();
		return out;
	}

	[[nodiscard]] constexpr bool operator==(Derived<T> const& other) const {
		for (size_t i = 0; i < N; ++i) {
			if (self_()[i] != other[i]) {
				return false;
			}
		}
		return true;
	}

	[[nodiscard]] constexpr bool operator!=(Derived<T> const& other) const { return !(self_() == other); }

	template <numeric U>
	[[nodiscard]] explicit constexpr operator Derived<U>() const {
		Derived<U> out;
		for (size_t i = 0; i < N; ++i) {
			out[i] = static_cast<U>(self_()[i]);
		}
		return out;
	}

private:
	template <indexed_numeric Other, typename Op>
	constexpr auto binary_(Other const& other, Op&& op) const -> Derived<std::common_type_t<T, typename Other::value_type>> {
		Derived<std::common_type_t<T, typename Other::value_type>> out;
		for (size_t i = 0; i < N; ++i) {
			out[i] = op(self_()[i], other[i]);
		}
		return out;
	}

	template <numeric U, typename Op>
	constexpr auto binary_(U other, Op&& op) const -> Derived<std::common_type_t<T, U>> {
		Derived<std::common_type_t<T, U>> out;
		for (size_t i = 0; i < N; ++i) {
			out[i] = op(self_()[i], other);
		}
		return out;
	}

	[[nodiscard]] constexpr Derived<T> const& self_() const { return static_cast<Derived<T> const&>(*this); }

	[[nodiscard]] constexpr Derived<T>& self_() { return static_cast<Derived<T>&>(*this); }
};

} // namespace aether