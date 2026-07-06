#pragma once
#include <concepts>
#include <cstddef>

namespace aether::ref_impl_ {

struct block final {
	void* ptr;
	size_t strong_count;
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

	constexpr ref(ref&& other) noexcept
	        : block_(other.block_) {
		other.block_ = nullptr;
	}

	template <std::derived_from<T> U>
	ref(ref<U> const& other) noexcept
	        : block_(other.block_) {
		if (block_) {
			++block_->strong_count;
		}
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
		return *static_cast<T*>(block_->ptr);
	}

	ref& operator=(ref const& other) {
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

	ref& operator=(ref&& other) {
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

	constexpr bool operator==(ref const& other) const {
		return block_ == other.block_;
	}

	constexpr bool operator!=(ref const& other) const {
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
		if (!block_) {
			return;
		}

		auto* old_block = block_;
		block_          = nullptr;

		if (--old_block->strong_count == 0) {
			old_block->deleter(old_block->ptr);
			old_block->ptr = nullptr;
			delete old_block;
		}
	}

	ref_impl_::block* block_;
};

} // namespace aether