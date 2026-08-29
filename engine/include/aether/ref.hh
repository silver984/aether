#pragma once
#include <concepts>
#include <cstddef>
#include <new>
#include <utility>

namespace aether::_ref_impl {

template <typename T>
concept is_self_referenceable = requires { typename T::self_referenceable_; };

struct shared_block final {
	void* ptr;
	void (*deleter)(void*);
	uint32_t strong_count;
	uint32_t weak_count;
};

struct unique_block final {
	void* ptr;
	void (*deleter)(void*);
};

} // namespace aether::_ref_impl

namespace aether {

template <typename T>
class unique_ref final {
	template <typename>
	friend class unique_ref;

public:
	unique_ref() noexcept
	        : ptr_(nullptr)
	        , block_(nullptr) {
	}

	unique_ref(std::nullptr_t) noexcept
	        : ptr_(nullptr)
	        , block_(nullptr) {
	}

	unique_ref(unique_ref const&) = delete;

	unique_ref(unique_ref&& other) noexcept
	        : ptr_(std::exchange(other.ptr_, nullptr))
	        , block_(std::exchange(other.block_, nullptr)) {
	}

	template <std::derived_from<T> Other>
	unique_ref(unique_ref<Other>&& other) noexcept
	        : ptr_(static_cast<T*>(std::exchange(other.ptr_, nullptr)))
	        , block_(std::exchange(other.block_, nullptr)) {
	}

	~unique_ref() noexcept {
		release();
	}

	template <typename... Args>
	[[nodiscard]] static unique_ref create(Args&&... args) noexcept {
		return new (std::nothrow) T(std::forward<Args>(args)...);
	}

	void release() noexcept {
		if (!block_) {
			return;
		}

		block_->deleter(block_->ptr);
		block_->ptr = nullptr;

		delete block_;

		ptr_   = nullptr;
		block_ = nullptr;
	}

	[[nodiscard]] T* get() const noexcept {
		return ptr_;
	}

	[[nodiscard]] T* operator->() const noexcept {
		return get();
	}

	[[nodiscard]] T& operator*() const noexcept {
		return *get();
	}

	explicit operator bool() const noexcept {
		return get() != nullptr;
	}

	unique_ref& operator=(unique_ref const&) = delete;

	unique_ref& operator=(unique_ref&& other) noexcept {
		if (this != &other) {
			return move_(other);
		}
		return *this;
	}

	template <std::derived_from<T> Other>
	unique_ref& operator=(unique_ref<Other>&& other) noexcept {
		return move_(other);
	}

	bool operator==(std::nullptr_t) const noexcept {
		return get() == nullptr;
	}

	bool operator!=(std::nullptr_t) const noexcept {
		return !(*this == nullptr);
	}

	template <std::derived_from<T> Other>
	bool operator==(unique_ref<Other> const& other) const noexcept {
		return get() == other.get();
	}

	template <std::derived_from<T> Other>
	bool operator!=(unique_ref<Other> const& other) const noexcept {
		return !(*this == other);
	}

private:
	unique_ref(T* ptr) noexcept {
		block_ = new (std::nothrow) _ref_impl::unique_block;

		if (!block_) {
			return;
		}

		ptr_            = ptr;
		block_->ptr     = ptr;
		block_->deleter = [](void* p) {
			delete static_cast<T*>(p);
		};
	}

	template <std::derived_from<T> Other>
	unique_ref& move_(unique_ref<Other>& other) noexcept {
		release();
		ptr_   = static_cast<T*>(std::exchange(other.ptr_, nullptr));
		block_ = std::exchange(other.block_, nullptr);
		return *this;
	}

	T* ptr_;
	_ref_impl::unique_block* block_;
};

template <typename>
class weak_ref;

template <typename>
class self_ref;

template <typename T>
class strong_ref final {
	template <typename>
	friend class strong_ref;

	template <typename>
	friend class weak_ref;

public:
	strong_ref() noexcept
	        : ptr_(nullptr)
	        , block_(nullptr) {
	}

	strong_ref(std::nullptr_t) noexcept
	        : ptr_(nullptr)
	        , block_(nullptr) {
	}

	strong_ref(strong_ref const& other) noexcept
	        : ptr_(other.ptr_)
	        , block_(other.block_) {
		inc_strong_();
	}

	template <std::derived_from<T> Other>
	strong_ref(strong_ref<Other> const& other) noexcept
	        : ptr_(static_cast<T*>(other.ptr_))
	        , block_(other.block_) {
		inc_strong_();
	}

	strong_ref(strong_ref&& other) noexcept
	        : ptr_(std::exchange(other.ptr_, nullptr))
	        , block_(std::exchange(other.block_, nullptr)) {
	}

	template <std::derived_from<T> Other>
	strong_ref(strong_ref<Other>&& other) noexcept
	        : ptr_(static_cast<T*>(std::exchange(other.ptr_, nullptr)))
	        , block_(std::exchange(other.block_, nullptr)) {
	}

	~strong_ref() noexcept {
		release();
	}

	template <typename... Args>
	[[nodiscard]] static strong_ref create(Args&&... args) {
		return new (std::nothrow) T(std::forward<Args>(args)...);
	}

	void release() noexcept {
		if (!block_) {
			return;
		}

		_ref_impl::shared_block* old_block = std::exchange(block_, nullptr);
		ptr_                               = nullptr;

		if (--old_block->strong_count != 0) {
			return;
		}

		old_block->deleter(old_block->ptr);
		old_block->ptr = nullptr;

		if (--old_block->weak_count == 0) {
			delete old_block;
		}
	}

	[[nodiscard]] T* get() const noexcept {
		return ptr_;
	}

	[[nodiscard]] T* operator->() const noexcept {
		return get();
	}

	[[nodiscard]] T& operator*() const noexcept {
		return *get();
	}

	[[nodiscard]] uint32_t strong_count() const noexcept {
		return block_ ? block_->strong_count : 0;
	}

	[[nodiscard]] uint32_t weak_count() const noexcept {
		// subtract implicit count
		return block_ ? (block_->weak_count - 1) : 0;
	}

	explicit operator bool() const noexcept {
		return get() != nullptr;
	}

	strong_ref& operator=(strong_ref const& other) noexcept {
		if (this != &other) {
			return copy_(other);
		}
		return *this;
	}

	template <std::derived_from<T> Other>
	strong_ref& operator=(strong_ref<Other> const& other) noexcept {
		return copy_(other);
	}

	strong_ref& operator=(strong_ref&& other) noexcept {
		if (this != &other) {
			return move_(other);
		}
		return *this;
	}

	template <std::derived_from<T> Other>
	strong_ref& operator=(strong_ref<Other>&& other) noexcept {
		return move_(other);
	}

	bool operator==(std::nullptr_t) const noexcept {
		return get() == nullptr;
	}

	bool operator!=(std::nullptr_t) const noexcept {
		return !(*this == nullptr);
	}

	template <std::derived_from<T> Other>
	bool operator==(strong_ref<Other> const& other) const noexcept {
		return get() == other.get();
	}

	template <std::derived_from<T> Other>
	bool operator!=(strong_ref<Other> const& other) const noexcept {
		return !(*this == other);
	}

private:
	strong_ref(T* ptr) noexcept {
		block_ = new (std::nothrow) _ref_impl::shared_block;

		if (!block_) {
			return;
		}

		ptr_                 = ptr;
		block_->ptr          = ptr;
		block_->strong_count = 1;
		block_->weak_count   = 1; // implicit count
		block_->deleter      = [](void* p) {
			delete static_cast<T*>(p);
		};

		if constexpr (_ref_impl::is_self_referenceable<T>) {
			if (ptr) {
				ptr->init_self_ref_(*this);
			}
		}
	}

	void inc_strong_() noexcept {
		if (block_) {
			++block_->strong_count;
		}
	}

	template <std::derived_from<T> Other>
	strong_ref& copy_(strong_ref<Other> const& other) noexcept {
		release();
		ptr_   = static_cast<T*>(other.ptr_);
		block_ = other.block_;
		inc_strong_();
		return *this;
	}

	template <std::derived_from<T> Other>
	strong_ref& move_(strong_ref<Other>& other) noexcept {
		release();
		ptr_   = static_cast<T*>(std::exchange(other.ptr_, nullptr));
		block_ = std::exchange(other.block_, nullptr);
		inc_strong_();
		return *this;
	}

	T* ptr_;
	_ref_impl::shared_block* block_;
};

template <typename T>
class weak_ref final {
	template <typename>
	friend class self_ref;

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

	weak_ref(strong_ref<T> const& other) noexcept
	        : block_(other.block_) {
		inc_weak_();
	}

	~weak_ref() noexcept {
		detach();
	}

	void detach() noexcept {
		if (!block_) {
			return;
		}
		_ref_impl::shared_block* old_block = std::exchange(block_, nullptr);
		if (--old_block->weak_count == 0 && old_block->strong_count == 0) {
			delete old_block;
		}
	}

	[[nodiscard]] strong_ref<T> construct() const noexcept {
		if (is_expired()) {
			return nullptr;
		}
		strong_ref<T> out;
		out.ptr_   = static_cast<T*>(block_->ptr);
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
		detach();
		return copy_(other.block_);
	}

	weak_ref& operator=(weak_ref&& other) noexcept {
		if (this == &other) {
			return *this;
		}
		detach();
		block_ = std::exchange(other.block_, nullptr);
		return *this;
	}

	weak_ref& operator=(strong_ref<T> const& other) noexcept {
		detach();
		return copy_(other.block_);
	}

private:
	void inc_weak_() noexcept {
		if (block_) {
			++block_->weak_count;
		}
	}

	weak_ref& copy_(_ref_impl::shared_block* block) noexcept {
		block_ = block;
		inc_weak_();
		return *this;
	}

	_ref_impl::shared_block* block_;
};

template <typename T>
class self_ref {
	template <typename>
	friend class weak_ref;

	template <typename>
	friend class strong_ref;

public:
	self_ref(self_ref const&)            = delete;
	self_ref(self_ref&&)                 = delete;
	self_ref& operator=(self_ref const&) = delete;
	self_ref& operator=(self_ref&&)      = delete;

protected:
	self_ref() noexcept          = default;
	virtual ~self_ref() noexcept = default;

	[[nodiscard]] strong_ref<T> strong_self_() const noexcept {
		return weak_.construct();
	}

	[[nodiscard]] weak_ref<T> weak_self_() const noexcept {
		return weak_;
	}

private:
	using self_referenceable_ = void;

	void init_self_ref_(strong_ref<T> const& ref) noexcept {
		// its expected that this function is only called once
		weak_ = ref;
	}

	weak_ref<T> mutable weak_;
};

} // namespace aether