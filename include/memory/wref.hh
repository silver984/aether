#pragma once
#include <memory/block.hh>

namespace aether {

template <typename T>
class sref;

template <typename T>
class wref final {
public:
	constexpr wref() noexcept
	        : block_(nullptr) {}

	constexpr wref(wref<T> const& other)
	        : block_(other.block_) {
		if (block_) {
			++block_->weak_count;
		}
	}

	constexpr wref(wref<T>&& other) noexcept
	        : block_(other.block_) {
		other.block_ = nullptr;
	}

	wref(sref<T> const& other)
	        : block_(other.block_) {
		if (block_) {
			++block_->weak_count;
		}
	}

	~wref() {
		release_();
	}

	[[nodiscard]] sref<T> lock() const {
		if (is_expired()) {
			return nullptr;
		}

		++block_->strong_count;
		sref<T> out;
		out.block_ = block_;
		return out;
	}

	[[nodiscard]] constexpr bool is_expired() const {
		return !block_ || block_->strong_count == 0;
	}

	wref& operator=(wref const& other) {
		if (this == &other) {
			return *this;
		}

		release_();
		return copy_(other.block_);
	}

	wref& operator=(wref&& other) noexcept {
		if (this == &other) {
			return *this;
		}

		release_();
		return move_(other.block_);
	}

	wref& operator=(sref<T> const& other) {
		release_();
		return copy_(other.block_);
	}

private:
	void release_() {
		if (!block_) {
			return;
		}

		if (--block_->weak_count == 0 && block_->strong_count == 0) {
			delete block_;
		}
	}

	wref& copy_(block<T>* const& b) {
		block_ = b;
		if (block_) {
			++block_->weak_count;
		}
		return *this;
	}

	wref& move_(block<T>*& b) {
		block_ = b;
		b      = nullptr;
		return *this;
	}

	block<T>* block_;
};

} // namespace aether