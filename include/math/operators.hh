#pragma once
#include <cstddef>
#include <math/numeric.hh>

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
		derived<T> out(self());

		for (std::size_t i = 0; i < N; ++i) {
			out[i] *= -1;
		}

		return out;
	}

	[[nodiscard]] constexpr derived<T> operator+(derived<T> rhs) const {
		derived<T> out(self());

		for (std::size_t i = 0; i < N; ++i) {
			out[i] += rhs[i];
		}

		return out;
	}

	template <numeric U>
	[[nodiscard]] constexpr derived<T> operator+(derived<U> rhs) const {
		derived<T> out(self());

		for (std::size_t i = 0; i < N; ++i) {
			out[i] += static_cast<T>(rhs[i]);
		}

		return out;
	}

	template <numeric U>
	[[nodiscard]] constexpr derived<T> operator+(U rhs) const {
		derived<T> out(self());

		for (std::size_t i = 0; i < N; ++i) {
			out[i] += static_cast<T>(rhs);
		}

		return out;
	}

	[[nodiscard]] constexpr derived<T> operator-(derived<T> rhs) const {
		derived<T> out(self());

		for (std::size_t i = 0; i < N; ++i) {
			out[i] -= rhs[i];
		}

		return out;
	}

	template <numeric U>
	[[nodiscard]] constexpr derived<T> operator-(derived<U> rhs) const {
		derived<T> out(self());

		for (std::size_t i = 0; i < N; ++i) {
			out[i] -= static_cast<T>(rhs[i]);
		}

		return out;
	}

	template <numeric U>
	[[nodiscard]] constexpr derived<T> operator-(U rhs) const {
		derived<T> out(self());

		for (std::size_t i = 0; i < N; ++i) {
			out[i] -= static_cast<T>(rhs);
		}

		return out;
	}

	[[nodiscard]] constexpr derived<T> operator*(derived<T> rhs) const {
		derived<T> out(self());

		for (std::size_t i = 0; i < N; ++i) {
			out[i] *= rhs[i];
		}

		return out;
	}

	template <numeric U>
	[[nodiscard]] constexpr derived<T> operator*(derived<U> rhs) const {
		derived<T> out(self());

		for (std::size_t i = 0; i < N; ++i) {
			out[i] *= static_cast<T>(rhs[i]);
		}

		return out;
	}

	template <numeric U>
	[[nodiscard]] constexpr derived<T> operator*(U rhs) const {
		derived<T> out(self());

		for (std::size_t i = 0; i < N; ++i) {
			out[i] *= static_cast<T>(rhs);
		}

		return out;
	}

	[[nodiscard]] constexpr derived<T> operator/(derived<T> rhs) const {
		derived<T> out(self());

		for (std::size_t i = 0; i < N; ++i) {
			out[i] /= rhs[i];
		}

		return out;
	}

	template <numeric U>
	[[nodiscard]] constexpr derived<T> operator/(derived<U> rhs) const {
		derived<T> out(self());

		for (std::size_t i = 0; i < N; ++i) {
			out[i] /= static_cast<T>(rhs[i]);
		}

		return out;
	}

	template <numeric U>
	[[nodiscard]] constexpr derived<T> operator/(U rhs) const {
		derived<T> out(self());

		for (std::size_t i = 0; i < N; ++i) {
			out[i] /= static_cast<T>(rhs);
		}

		return out;
	}

	constexpr derived<T>& operator+=(derived<T> rhs) {
		for (std::size_t i = 0; i < N; ++i) {
			self()[i] += rhs[i];
		}

		return self();
	}

	template <numeric U>
	constexpr derived<T>& operator+=(derived<U> rhs) {
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

	constexpr derived<T>& operator-=(derived<T> rhs) {
		for (std::size_t i = 0; i < N; ++i) {
			self()[i] -= rhs[i];
		}

		return self();
	}

	template <numeric U>
	constexpr derived<T>& operator-=(derived<U> rhs) {
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

	constexpr derived<T>& operator*=(derived<T> rhs) {
		for (std::size_t i = 0; i < N; ++i) {
			self()[i] *= rhs[i];
		}

		return self();
	}

	template <numeric U>
	constexpr derived<T>& operator*=(derived<U> rhs) {
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

	constexpr derived<T>& operator/=(derived<T> rhs) {
		for (std::size_t i = 0; i < N; ++i) {
			self()[i] /= rhs[i];
		}

		return self();
	}

	template <numeric U>
	constexpr derived<T>& operator/=(derived<U> rhs) {
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
		derived<U> out;

		for (std::size_t i = 0; i < N; ++i) {
			out[i] = static_cast<U>(self()[i]);
		}

		return out;
	}

private:
	[[nodiscard]] constexpr derived<T> const& self() const {
		return static_cast<derived<T> const&>(*this);
	}

	[[nodiscard]] constexpr derived<T>& self() {
		return static_cast<derived<T>&>(*this);
	}
};

} // namespace aether