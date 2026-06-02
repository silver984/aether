#pragma once
#include <cstddef>
#include <math/numeric.hh>

namespace aether {

template <template <typename> class derived, numeric T, std::size_t N>
struct operators {
	[[nodiscard]] constexpr derived<T> operator+() const {
		return self();
	}

	[[nodiscard]] constexpr derived<T> operator-() const {
		derived<T> r(self());

		for (std::size_t i = 0; i < N; ++i) {
			r[i] *= -1;
		}

		return r;
	}

	[[nodiscard]] constexpr derived<T> operator+(derived<T> rhs) const {
		derived<T> r(self());

		for (std::size_t i = 0; i < N; ++i) {
			r[i] += rhs[i];
		}

		return r;
	}

	template <numeric U>
	[[nodiscard]] constexpr derived<T> operator+(derived<U> rhs) const {
		derived<T> r(self());

		for (std::size_t i = 0; i < N; ++i) {
			r[i] += static_cast<T>(rhs[i]);
		}

		return r;
	}

	template <numeric U>
	[[nodiscard]] constexpr derived<T> operator+(U rhs) const {
		derived<T> r(self());

		for (std::size_t i = 0; i < N; ++i) {
			r[i] += static_cast<T>(rhs);
		}

		return r;
	}

	[[nodiscard]] constexpr derived<T> operator-(derived<T> rhs) const {
		derived<T> r(self());

		for (std::size_t i = 0; i < N; ++i) {
			r[i] -= rhs[i];
		}

		return r;
	}

	template <numeric U>
	[[nodiscard]] constexpr derived<T> operator-(derived<U> rhs) const {
		derived<T> r(self());

		for (std::size_t i = 0; i < N; ++i) {
			r[i] -= static_cast<T>(rhs[i]);
		}

		return r;
	}

	template <numeric U>
	[[nodiscard]] constexpr derived<T> operator-(U rhs) const {
		derived<T> r(self());

		for (std::size_t i = 0; i < N; ++i) {
			r[i] -= static_cast<T>(rhs);
		}

		return r;
	}

	[[nodiscard]] constexpr derived<T> operator*(derived<T> rhs) const {
		derived<T> r(self());

		for (std::size_t i = 0; i < N; ++i) {
			r[i] *= rhs[i];
		}

		return r;
	}

	template <numeric U>
	[[nodiscard]] constexpr derived<T> operator*(derived<U> rhs) const {
		derived<T> r(self());

		for (std::size_t i = 0; i < N; ++i) {
			r[i] *= static_cast<T>(rhs[i]);
		}

		return r;
	}

	template <numeric U>
	[[nodiscard]] constexpr derived<T> operator*(U rhs) const {
		derived<T> r(self());

		for (std::size_t i = 0; i < N; ++i) {
			r[i] *= static_cast<T>(rhs);
		}

		return r;
	}

	[[nodiscard]] constexpr derived<T> operator/(derived<T> rhs) const {
		derived<T> r(self());

		for (std::size_t i = 0; i < N; ++i) {
			r[i] /= rhs[i];
		}

		return r;
	}

	template <numeric U>
	[[nodiscard]] constexpr derived<T> operator/(derived<U> rhs) const {
		derived<T> r(self());

		for (std::size_t i = 0; i < N; ++i) {
			r[i] /= static_cast<T>(rhs[i]);
		}

		return r;
	}

	template <numeric U>
	[[nodiscard]] constexpr derived<T> operator/(U rhs) const {
		derived<T> r(self());

		for (std::size_t i = 0; i < N; ++i) {
			r[i] /= static_cast<T>(rhs);
		}

		return r;
	}

	constexpr derived<T>& operator+=(derived<T> rhs) {
		for (std::size_t i = 0; i < N; ++i) {
			self()[i] += rhs[i];
		}

		return self();
	}

	template <typename U>
	constexpr derived<T>& operator+=(derived<U> rhs) {
		for (std::size_t i = 0; i < N; ++i) {
			self()[i] += static_cast<T>(rhs[i]);
		}

		return self();
	}

	template <typename U>
	constexpr derived<T>& operator+=(U rhs) {
		for (std::size_t i = 0; i < N; ++i) {
			self()[i] += static_cast<T>(rhs);
		}

		return self();
	}

	constexpr derived<T>& operator-=(derived<T> rhs) {
		for (std::size_t i = 0; i < N; ++i) {
			self()[i] -= rhs[i];
		}

		return self();
	}

	template <typename U>
	constexpr derived<T>& operator-=(derived<U> rhs) {
		for (std::size_t i = 0; i < N; ++i) {
			self()[i] -= static_cast<T>(rhs[i]);
		}

		return self();
	}

	template <typename U>
	constexpr derived<T>& operator-=(U rhs) {
		for (std::size_t i = 0; i < N; ++i) {
			self()[i] -= static_cast<T>(rhs);
		}

		return self();
	}

	constexpr derived<T>& operator*=(derived<T> rhs) {
		for (std::size_t i = 0; i < N; ++i) {
			self()[i] *= rhs[i];
		}

		return self();
	}

	template <typename U>
	constexpr derived<T>& operator*=(derived<U> rhs) {
		for (std::size_t i = 0; i < N; ++i) {
			self()[i] *= static_cast<T>(rhs[i]);
		}

		return self();
	}

	template <typename U>
	constexpr derived<T>& operator*=(U rhs) {
		for (std::size_t i = 0; i < N; ++i) {
			self()[i] *= static_cast<T>(rhs);
		}

		return self();
	}

	constexpr derived<T>& operator/=(derived<T> rhs) {
		for (std::size_t i = 0; i < N; ++i) {
			self()[i] /= rhs[i];
		}

		return self();
	}

	template <typename U>
	constexpr derived<T>& operator/=(derived<U> rhs) {
		for (std::size_t i = 0; i < N; ++i) {
			self()[i] /= static_cast<T>(rhs[i]);
		}

		return self();
	}

	template <typename U>
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
		derived<T> r(self());
		++self();
		return r;
	}

	constexpr derived<T>& operator--() {
		for (std::size_t i = 0; i < N; ++i) {
			--self()[i];
		}

		return self();
	}

	constexpr derived<T> operator--(int) {
		derived<T> r(self());
		--self();
		return r;
	}

	[[nodiscard]] constexpr bool operator==(derived<T> rhs) const {
		for (std::size_t i = 0; i < N; ++i) {
			if (self()[i] != rhs[i]) {
				return false;
			}
		}

		return true;
	}

	[[nodiscard]] constexpr bool operator!=(derived<T> rhs) const {
		return !(self() == rhs);
	}

	template <numeric U>
	[[nodiscard]] explicit constexpr operator derived<U>() const {
		derived<U> r;

		for (std::size_t i = 0; i < N; ++i) {
			r[i] = static_cast<U>(self()[i]);
		}

		return r;
	}

private:
	[[nodiscard]] constexpr derived<T> const& self() const {
		return static_cast<derived<T> const&>(*this);
	}

	[[nodiscard]] constexpr derived<T>& self() {
		return static_cast<derived<T>&>(*this);
	}
};

} // namespace ae