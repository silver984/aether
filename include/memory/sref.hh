#pragma once
#include <concepts>
#include <cstddef>
#include <memory/block.hh>

namespace aether {

template <typename T>
class wref;

template <typename T>
class self_ref;

template <typename T>
class sref final {
	friend class wref<T>;

public:
	constexpr sref() noexcept
	        : block_(nullptr) {}

	constexpr sref(std::nullptr_t) noexcept
	        : block_(nullptr) {}

	sref(T* ptr)
	        : block_(new block<T>()) {
		block_->ptr          = ptr;
		block_->strong_count = 1;
		block_->weak_count   = 0;

		if constexpr (std::derived_from<T, self_ref<T>>) {
			ptr->weak_this_ = *this;
		}
	}

	sref(sref const& other)
	        : block_(other.block_) {
		if (block_) {
			++block_->strong_count;
		}
	}

	constexpr sref(sref&& other) noexcept
	        : block_(other.block_) {
		other.block_ = nullptr;
	}

	~sref() {
		release_();
	}

	[[nodiscard]] constexpr T* get() const {
		return operator->();
	}

	[[nodiscard]] constexpr size_t strong_count() const {
		return block_ ? block_->strong_count : 0;
	}

	[[nodiscard]] constexpr size_t weak_count() const {
		return block_ ? block_->weak_count : 0;
	}

	constexpr T* operator->() const {
		return block_ ? block_->ptr : nullptr;
	}

	constexpr T& operator*() const {
		return *block_->ptr;
	}

	sref& operator=(sref const& other) {
		if (this == &other) {
			return *this;
		}

		release_();
		block_ = other.block_;

		if (block_) {
			++block_->strong_count;
		}

		return *this;
	}

	sref& operator=(sref&& other) {
		if (this == &other) {
			return *this;
		}

		release_();
		block_       = other.block_;
		other.block_ = nullptr;
		return *this;
	}

	constexpr explicit operator bool() const {
		return operator!=(nullptr);
	}

	constexpr bool operator==(sref const& other) const {
		return block_ == other.block_;
	}

	constexpr bool operator!=(sref const& other) const {
		return !(*this == other);
	}

	constexpr bool operator==(std::nullptr_t) const {
		return block_ == nullptr;
	}

	constexpr bool operator!=(std::nullptr_t) const {
		return block_ != nullptr;
	}

private:
	void release_() {
		if (!block_ || block_->strong_count == 0) {
			return;
		}

		if (--block_->strong_count == 0) {
			delete block_->ptr;
			block_->ptr = nullptr;

			if (block_->weak_count == 0) {
				delete block_;
			}
		}
	}

	block<T>* block_;
};

} // namespace aether