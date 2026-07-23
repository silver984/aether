#pragma once
#include <concepts>
#include <cstdint>

namespace aether::ref_impl_ {

struct block final {
	void* ptr;
	uint32_t strong_count;
	void (*deleter)(void*);
};

} // namespace aether::ref_impl_

namespace aether {

template <typename T>
class ref final {
	template <typename>
	friend class ref;

public:
	constexpr ref() noexcept
	        : block_(nullptr) {
	}

	constexpr ref(std::nullptr_t) noexcept
	        : block_(nullptr) {
	}

	template <std::derived_from<T> U>
	ref(U* ptr)
	        : block_(new ref_impl_::block{.ptr = ptr, .strong_count = 1, .deleter = [](void* p) {
		                                      delete static_cast<U*>(p);
	                                      }}) {
	}

	ref(ref const& other) noexcept
	        : block_(other.block_) {
		if (block_) {
			++block_->strong_count;
		}
	}

	template <std::derived_from<T> U>
	ref(ref<U> const& other) noexcept
	        : block_(other.block_) {
		if (block_) {
			++block_->strong_count;
		}
	}

	constexpr ref(ref&& other) noexcept
	        : block_(other.block_) {
		other.block_ = nullptr;
	}

	template <std::derived_from<T> U>
	constexpr ref(ref<U>&& other) noexcept
	        : block_(other.block_) {
		other.block_ = nullptr;
	}

	~ref() {
		release_();
	}

	void nullify() {
		release_();
	}

	[[nodiscard]] constexpr T* get() const {
		return operator->();
	}

	[[nodiscard]] constexpr size_t strong_count() const {
		return block_ ? block_->strong_count : 0;
	}

	constexpr T* operator->() const {
		return block_ ? static_cast<T*>(block_->ptr) : nullptr;
	}

	constexpr T& operator*() const {
		return *operator->();
	}

	ref& operator=(ref const& other) {
		if (this == &other) {
			return *this;
		}
		return copy_(other.block_);
	}

	template <std::derived_from<T> U>
	ref& operator=(ref<U> const& other) {
		if (this == &other) {
			return *this;
		}
		return copy_(other.block_);
	}

	ref& operator=(ref&& other) {
		if (this == &other) {
			return *this;
		}
		return move_(other.block_);
	}

	template <std::derived_from<T> U>
	ref& operator=(ref<U>&& other) {
		if (this == &other) {
			return *this;
		}
		return move_(other.block_);
	}

	constexpr explicit operator bool() const {
		return operator!=(nullptr);
	}

	template <std::derived_from<T> U>
	constexpr bool operator==(ref<U> const& other) const {
		return get() == other.get();
	}

	template <std::derived_from<T> U>
	constexpr bool operator!=(ref const& other) const {
		return get() != other.get();
	}

	constexpr bool operator==(std::nullptr_t) const {
		return get() == nullptr;
	}

	constexpr bool operator!=(std::nullptr_t) const {
		return get() != nullptr;
	}

private:
	void release_() {
		if (!block_) {
			return;
		}
		ref_impl_::block* old_block = block_;
		block_                      = nullptr;
		if (--old_block->strong_count == 0) {
			old_block->deleter(old_block->ptr);
			old_block->ptr = nullptr;
			delete old_block;
		}
	}

	ref& copy_(ref_impl_::block* block) {
		release_();
		block_ = block;
		if (block_) {
			++block_->strong_count;
		}
		return *this;
	}

	ref& move_(ref_impl_::block*& block) {
		release_();
		block_ = block;
		block  = nullptr;
		return *this;
	}

	ref_impl_::block* block_;
};

} // namespace aether