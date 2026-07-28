#pragma once
#include <concepts>
#include <cstddef>
#include <new>
#include <utility>

namespace aether::ref_impl_ {

struct block final {
	void* ptr;
	void (*deleter)(void*);
	uint32_t strong_count;
};

} // namespace aether::ref_impl_

namespace aether {

template <typename Type>
class ref final {
	template <typename>
	friend class ref;

	template <typename>
	friend class wref;

public:
	ref() noexcept
	        : block_(nullptr) {
	}

	ref(std::nullptr_t) noexcept
	        : block_(nullptr) {
	}

	ref(Type* ptr) noexcept
	        : block_(new (std::nothrow) ref_impl_::block{
	                  .ptr = ptr,
	                  .deleter =
	                          [](void* p) {
		                          delete static_cast<Type*>(p);
	                          },
	                  .strong_count = 1,
	          }) {
	}

	template <std::derived_from<Type> DerivedType>
	ref(DerivedType* ptr) noexcept
	        : block_(new (std::nothrow) ref_impl_::block{
	                  .ptr = ptr,
	                  .deleter =
	                          [](void* p) {
		                          delete static_cast<DerivedType*>(p);
	                          },
	                  .strong_count = 1,
	          }) {
	}

	ref(ref const& other) noexcept
	        : block_(other.block_) {
		inc_strong_();
	}

	template <std::derived_from<Type> OtherType>
	ref(ref<OtherType> const& other) noexcept
	        : block_(other.block_) {
		inc_strong_();
	}

	ref(ref&& other) noexcept
	        : block_(std::exchange(other.block_, nullptr)) {
	}

	template <std::derived_from<Type> OtherType>
	ref(ref<OtherType>&& other) noexcept
	        : block_(std::exchange(other.block_, nullptr)) {
	}

	~ref() noexcept {
		release_();
	}

	void nullify() noexcept {
		release_();
	}

	[[nodiscard]] Type* get() const noexcept {
		return block_ ? static_cast<Type*>(block_->ptr) : nullptr;
	}

	[[nodiscard]] Type* operator->() const noexcept {
		return get();
	}

	[[nodiscard]] Type& operator*() const noexcept {
		return *get();
	}

	[[nodiscard]] uint32_t strong_count() const noexcept {
		return block_ ? block_->strong_count : 0;
	}

	explicit operator bool() const noexcept {
		return get() != nullptr;
	}

	ref& operator=(ref const& other) noexcept {
		if (this != &other) {
			assign_(other.block_);
		}
		return *this;
	}

	template <std::derived_from<Type> OtherType>
	ref& operator=(ref<OtherType> const& other) noexcept {
		assign_(other.block_);
		return *this;
	}

	ref& operator=(ref&& other) noexcept {
		if (this != &other) {
			assign_(std::exchange(other.block_, nullptr));
		}
		return *this;
	}

	template <std::derived_from<Type> OtherType>
	ref& operator=(ref<OtherType>&& other) noexcept {
		assign_(std::exchange(other.block_, nullptr));
		return *this;
	}

	bool operator==(std::nullptr_t) const noexcept {
		return get() == nullptr;
	}

	bool operator!=(std::nullptr_t) const noexcept {
		return get() != nullptr;
	}

	template <std::derived_from<Type> OtherType>
	bool operator==(ref<OtherType> const& other) const noexcept {
		return get() == other.get();
	}

	template <std::derived_from<Type> OtherType>
	bool operator!=(ref<OtherType> const& other) const noexcept {
		return !(*this == other);
	}

private:
	void inc_strong_() noexcept {
		if (block_) {
			++block_->strong_count;
		}
	}

	void release_() noexcept {
		if (!block_) {
			return;
		}

		ref_impl_::block* old_block = std::exchange(block_, nullptr);

		if (--old_block->strong_count != 0) {
			return;
		}

		old_block->deleter(old_block->ptr);
		old_block->ptr = nullptr;
		delete old_block;
	}

	void assign_(ref_impl_::block* new_block) noexcept {
		if (block_ == new_block) {
			return;
		}
		release_();
		block_ = new_block;
		inc_strong_();
	}

	ref_impl_::block* block_;
};

template <typename Type, typename... Args>
ref<Type> make_ref(Args&&... args) {
	return new Type(std::forward<Args>(args)...);
}

} // namespace aether