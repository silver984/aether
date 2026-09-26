#pragma once

#include "ref-impl/block.hh"

#include <concepts>
#include <cstddef>
#include <utility>

namespace aether::_ref_impl {
template <typename T_>
concept self_referenceable_ = requires { typename T_::_self_referenceable; };
} // namespace aether::_ref_impl

namespace aether {

template <typename>
class weak_ref;

template <typename T>
class strong_ref final {
	template <typename>
	friend class strong_ref;

	template <typename>
	friend class weak_ref;

public:
	strong_ref()
	        : ptr_(nullptr)
	        , block_(nullptr) {}

	strong_ref(std::nullptr_t)
	        : ptr_(nullptr)
	        , block_(nullptr) {}

	strong_ref(strong_ref const& other)
	        : ptr_(other.ptr_)
	        , block_(other.block_) {
		inc_strong_();
	}

	template <std::derived_from<T> Other>
	strong_ref(strong_ref<Other> const& other)
	        : ptr_(static_cast<T*>(other.ptr_))
	        , block_(other.block_) {
		inc_strong_();
	}

	strong_ref(strong_ref&& other)
	        : ptr_(std::exchange(other.ptr_, nullptr))
	        , block_(std::exchange(other.block_, nullptr)) {}

	template <std::derived_from<T> Other>
	strong_ref(strong_ref<Other>&& other)
	        : ptr_(static_cast<T*>(std::exchange(other.ptr_, nullptr)))
	        , block_(std::exchange(other.block_, nullptr)) {}

	~strong_ref() { release(); }

	template <typename... Args>
	[[nodiscard]] static strong_ref create(Args&&... args) {
		return new (std::nothrow) T(std::forward<Args>(args)...);
	}

	void release() {
		if (!block_) {
			return;
		}

		_ref_impl::shared_block_* old_block = std::exchange(block_, nullptr);
		ptr_                                = nullptr;

		if (--old_block->strong_count != 0) {
			return;
		}

		old_block->releaser(old_block->ptr);
		old_block->ptr = nullptr;

		if (--old_block->weak_count == 0) {
			delete old_block;
		}
	}

	[[nodiscard]] T* get() const { return ptr_; }
	[[nodiscard]] T* operator->() const { return get(); }
	[[nodiscard]] T& operator*() const { return *get(); }
	[[nodiscard]] uint32_t strong_count() const { return block_ ? block_->strong_count : 0; }

	[[nodiscard]] uint32_t weak_count() const {
		// subtract implicit count
		return block_ ? (block_->weak_count - 1) : 0;
	}

	explicit operator bool() const { return get() != nullptr; }

	strong_ref& operator=(strong_ref const& other) {
		if (this != &other) {
			return copy_(other);
		}
		return *this;
	}

	template <std::derived_from<T> Other>
	strong_ref& operator=(strong_ref<Other> const& other) {
		return copy_(other);
	}

	strong_ref& operator=(strong_ref&& other) {
		if (this != &other) {
			return move_(other);
		}
		return *this;
	}

	template <std::derived_from<T> Other>
	strong_ref& operator=(strong_ref<Other>&& other) {
		return move_(other);
	}

	bool operator==(std::nullptr_t) const { return get() == nullptr; }
	bool operator!=(std::nullptr_t) const { return !(*this == nullptr); }

	template <std::derived_from<T> Other>
	bool operator==(strong_ref<Other> const& other) const {
		return get() == other.get();
	}

	template <std::derived_from<T> Other>
	bool operator!=(strong_ref<Other> const& other) const {
		return !(*this == other);
	}

private:
	strong_ref(T* ptr) {
		block_ = new (std::nothrow) _ref_impl::shared_block_;

		if (!block_) {
			return;
		}

		ptr_                 = ptr;
		block_->ptr          = ptr;
		block_->strong_count = 1;
		block_->weak_count   = 1; // implicit count
		block_->releaser     = [](void* p) {
			delete static_cast<T*>(p);
		};

		if constexpr (_ref_impl::self_referenceable_<T>) {
			if (ptr) {
				ptr->init_self_ref_(*this);
			}
		}
	}

	void inc_strong_() {
		if (block_) {
			++block_->strong_count;
		}
	}

	template <std::derived_from<T> Other_>
	strong_ref& copy_(strong_ref<Other_> const& other) {
		release();
		ptr_   = static_cast<T*>(other.ptr_);
		block_ = other.block_;
		inc_strong_();
		return *this;
	}

	template <std::derived_from<T> Other_>
	strong_ref& move_(strong_ref<Other_>& other) {
		release();
		ptr_   = static_cast<T*>(std::exchange(other.ptr_, nullptr));
		block_ = std::exchange(other.block_, nullptr);
		inc_strong_();
		return *this;
	}

	T* ptr_;
	_ref_impl::shared_block_* block_;
};

} // namespace aether