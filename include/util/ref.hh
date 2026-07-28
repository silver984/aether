#pragma once
#include <concepts>
#include <cstddef>
#include <utility>

namespace aether::ref_impl_ {

struct ref_block final {
	void* ptr;
	void (*deleter)(void*);
	uint32_t strong_count;
	uint32_t weak_count;
};

} // namespace aether::ref_impl_

namespace aether {

template <typename>
class weak_ref;
template <typename>
class this_ref;

template <typename Type>
class strong_ref final {
	template <typename>
	friend class strong_ref;
	template <typename>
	friend class weak_ref;

public:
	strong_ref() noexcept
	        : block_(nullptr) {
	}

	strong_ref(std::nullptr_t) noexcept
	        : block_(nullptr) {
	}

	explicit strong_ref(Type* ptr)
	        : block_(new ref_impl_::ref_block{
	                  .ptr = ptr,
	                  .deleter =
	                          [](void* ptr) {
		                          delete static_cast<Type*>(ptr);
	                          },
	                  .strong_count = 1,
	                  .weak_count   = 0,
	          }) {
		init_weak_this_(ptr);
	}

	template <std::derived_from<Type> DerivedType>
	explicit strong_ref(DerivedType* ptr)
	        : block_(new ref_impl_::ref_block{
	                  .ptr = ptr,
	                  .deleter =
	                          [](void* ptr) {
		                          delete static_cast<DerivedType*>(ptr);
	                          },
	                  .strong_count = 1,
	                  .weak_count   = 0,
	          }) {
		init_weak_this_(ptr);
	}

	strong_ref(strong_ref const& other) noexcept
	        : block_(other.block_) {
		inc_strong_();
	}

	template <std::derived_from<Type> OtherType>
	strong_ref(strong_ref<OtherType> const& other) noexcept
	        : block_(other.block_) {
		inc_strong_();
	}

	strong_ref(strong_ref&& other) noexcept
	        : block_(std::exchange(other.block_, nullptr)) {
	}

	template <std::derived_from<Type> OtherType>
	strong_ref(strong_ref<OtherType>&& other) noexcept
	        : block_(std::exchange(other.block_, nullptr)) {
	}

	~strong_ref() noexcept {
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

	[[nodiscard]] uint32_t weak_count() const noexcept {
		return block_ ? block_->weak_count : 0;
	}

	explicit operator bool() const noexcept {
		return get() != nullptr;
	}

	strong_ref& operator=(strong_ref const& other) noexcept {
		if (this != &other) {
			assign_(other.block_);
		}
		return *this;
	}

	template <std::derived_from<Type> OtherType>
	strong_ref& operator=(strong_ref<OtherType> const& other) noexcept {
		assign_(other.block_);
		return *this;
	}

	strong_ref& operator=(strong_ref&& other) noexcept {
		if (this != &other) {
			assign_(std::exchange(other.block_, nullptr));
		}
		return *this;
	}

	template <std::derived_from<Type> OtherType>
	strong_ref& operator=(strong_ref<OtherType>&& other) noexcept {
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
	bool operator==(strong_ref<OtherType> const& other) const noexcept {
		return get() == other.get();
	}

	template <std::derived_from<Type> OtherType>
	bool operator!=(strong_ref<OtherType> const& other) const noexcept {
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

		ref_impl_::ref_block* old_block = std::exchange(block_, nullptr);

		if (--old_block->strong_count != 0) {
			return;
		}

		old_block->deleter(old_block->ptr);
		old_block->ptr = nullptr;

		if (old_block->weak_count == 0) {
			delete old_block;
		}
	}

	void assign_(ref_impl_::ref_block* new_block) noexcept {
		if (block_ == new_block) {
			return;
		}
		release_();
		block_ = new_block;
		inc_strong_();
	}

	void init_weak_this_(Type* ptr) noexcept {
		if constexpr (std::derived_from<Type, this_ref<Type>>) {
			if (ptr) {
				ptr->set_weak_ref_(block_);
			}
		}
	}

	ref_impl_::ref_block* block_;
};

template <typename Type, typename... Args>
[[nodiscard]] strong_ref<Type> make_strong_ref(Args&&... args) {
	return strong_ref<Type>(new Type(std::forward<Args>(args)...));
}

template <typename Type>
class weak_ref final {
	template <typename>
	friend class this_ref;

public:
	weak_ref() noexcept
	        : block_(nullptr) {
	}

	weak_ref(weak_ref const& other) noexcept
	        : block_(other.block_) {
		inc_weak_();
	}

	weak_ref(weak_ref&& other) noexcept
	        : block_(other.block_) {
		other.block_ = nullptr;
	}

	weak_ref(strong_ref<Type> const& other) noexcept
	        : block_(other.block_) {
		inc_weak_();
	}

	~weak_ref() noexcept {
		release_();
	}

	void detach() noexcept {
		release_();
	}

	[[nodiscard]] strong_ref<Type> construct() const noexcept {
		if (is_expired()) {
			return nullptr;
		}
		strong_ref<Type> out;
		out.block_ = block_;
		out.inc_strong_();
		return out;
	}

	[[nodiscard]] bool is_expired() const noexcept {
		return block_ == nullptr || block_->ptr == nullptr;
	}

	weak_ref& operator=(weak_ref const& other) noexcept {
		if (this == &other) {
			return *this;
		}
		release_();
		return copy_(other.block_);
	}

	weak_ref& operator=(weak_ref&& other) noexcept {
		if (this == &other) {
			return *this;
		}
		release_();
		block_ = std::exchange(other.block_, nullptr);
		return *this;
	}

	weak_ref& operator=(strong_ref<Type> const& other) noexcept {
		release_();
		return copy_(other.block_);
	}

private:
	void inc_weak_() noexcept {
		if (block_) {
			++block_->weak_count;
		}
	}

	void release_() noexcept {
		if (!block_) {
			return;
		}

		ref_impl_::ref_block* old_block = block_;
		block_                          = nullptr;

		if (--old_block->weak_count == 0 && old_block->strong_count == 0) {
			delete old_block;
		}
	}

	weak_ref& copy_(ref_impl_::ref_block* b) noexcept {
		block_ = b;
		inc_weak_();
		return *this;
	}

	ref_impl_::ref_block* block_;
};

template <typename Type>
class this_ref {
	template <typename>
	friend class weak_ref;
	template <typename>
	friend class strong_ref;

public:
	this_ref(this_ref const&) noexcept
	        : weak_() {
	}

	this_ref(this_ref&&) noexcept
	        : weak_() {
	}

	this_ref& operator=(this_ref const&) noexcept {
		return *this;
	}

	this_ref& operator=(this_ref&&) noexcept {
		return *this;
	}

protected:
	this_ref() noexcept          = default;
	virtual ~this_ref() noexcept = default;

	[[nodiscard]] strong_ref<Type> strong_this_() const noexcept {
		return weak_.construct();
	}

	[[nodiscard]] weak_ref<Type> weak_this_() const noexcept {
		return weak_;
	}

private:
	void set_weak_ref_(ref_impl_::ref_block* block) noexcept {
		weak_.block_ = block;
		weak_.inc_weak_();
	}

	weak_ref<Type> mutable weak_;
};

} // namespace aether