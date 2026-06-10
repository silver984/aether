#pragma once
#include <cstddef>

namespace aether {

template <typename T>
class ref final {
public:
	ref()
	        : block_(nullptr) {}

	ref(std::nullptr_t)
	        : block_(nullptr) {}

	explicit ref(T* ptr)
	        : block_(new block()) {
		reset_block_(ptr);
	}

	ref(ref const& other)
	        : block_(other.block_) {
		if (block_) {
			++block_->count;
		}
	}

	ref(ref&& other)
	        : block_(other.block_) {
		other.block_ = nullptr;
	}

	~ref() {
		release_();
	}

	[[nodiscard]] T* get() const {
		return operator->();
	}

	[[nodiscard]] size_t count() const {
		return block_ ? block_->count : 0;
	}

	T* operator->() const {
		return block_ ? block_->ptr : nullptr;
	}

	T& operator*() const {
		return *block_->ptr;
	}

	ref& operator=(ref const& other) {
		if (this == &other) {
			return *this;
		}

		release_();

		if (block_ = other.block_) {
			++block_->count;
		}

		return *this;
	}

	ref& operator=(ref&& other) {
		if (this == &other) {
			return *this;
		}

		release_();
		block_       = other.block_;
		other.block_ = nullptr;
		return *this;
	}

	explicit operator bool() const {
		return operator!=(nullptr);
	}

	bool operator==(ref const& other) const {
		return block_ == other.block_;
	}

	bool operator!=(ref const& other) const {
		return !(*this == other);
	}

	bool operator==(std::nullptr_t) const {
		return block_ == nullptr;
	}

	bool operator!=(std::nullptr_t) const {
		return block_ != nullptr;
	}

private:
	struct block final {
		T* ptr;
		size_t count;
	};

	void release_() {
		if (!block_ || block_->count == 0) {
			return;
		}

		if (--block_->count == 0) {
			delete block_->ptr;
			delete block_;
		}
	}

	void reset_block_(T* ptr) {
		block_->ptr   = ptr;
		block_->count = 1;
	}

	block* block_;
};

}