#pragma once

#include "strong_ref.hh"

namespace aether {

template <typename>
class self_ref;

template <typename T>
class weak_ref final {
	template <typename>
	friend class self_ref;

public:
	weak_ref() = default;

	weak_ref(weak_ref const& other)
	        : block_(other.block_) {
		inc_weak_();
	}

	weak_ref(weak_ref&& other)
	        : block_(other.block_) {
		other.block_ = nullptr;
	}

	weak_ref(strong_ref<T> const& other)
	        : block_(other.block_) {
		inc_weak_();
	}

	~weak_ref() { detach(); }

	void detach() {
		if (!block_) {
			return;
		}
		_ref_impl::shared_block_* old_block = std::exchange(block_, nullptr);
		if (--old_block->weak_count == 0 && old_block->strong_count == 0) {
			delete old_block;
		}
	}

	[[nodiscard]] strong_ref<T> construct() const {
		if (is_expired()) {
			return nullptr;
		}
		strong_ref<T> out;
		out.ptr_   = static_cast<T*>(block_->ptr);
		out.block_ = block_;
		out.inc_strong_();
		return out;
	}

	[[nodiscard]] bool is_expired() const { return block_ == nullptr || block_->ptr == nullptr; }

	weak_ref& operator=(weak_ref const& other) {
		if (this == &other) {
			return *this;
		}
		detach();
		return copy_(other.block_);
	}

	weak_ref& operator=(weak_ref&& other) {
		if (this == &other) {
			return *this;
		}
		detach();
		block_ = std::exchange(other.block_, nullptr);
		return *this;
	}

	weak_ref& operator=(strong_ref<T> const& other) {
		detach();
		return copy_(other.block_);
	}

private:
	void inc_weak_() {
		if (block_) {
			++block_->weak_count;
		}
	}

	weak_ref& copy_(_ref_impl::shared_block_* block) {
		block_ = block;
		inc_weak_();
		return *this;
	}

	_ref_impl::shared_block_* block_ = nullptr;
};

} // namespace aether