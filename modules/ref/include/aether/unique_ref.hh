#pragma once

#include "ref-impl/block.hh"

#include <concepts>
#include <cstddef>
#include <utility>

namespace aether {

struct ref;

template <typename T>
class unique_ref final {
	friend class ref;

	template <typename>
	friend class unique_ref;

public:
	unique_ref()
	        : ptr_(nullptr)
	        , block_(nullptr) {}

	unique_ref(std::nullptr_t)
	        : ptr_(nullptr)
	        , block_(nullptr) {}

	unique_ref(unique_ref const&) = delete;

	unique_ref(unique_ref&& other)
	        : ptr_(std::exchange(other.ptr_, nullptr))
	        , block_(std::exchange(other.block_, nullptr)) {}

	template <std::derived_from<T> Other>
	unique_ref(unique_ref<Other>&& other)
	        : ptr_(static_cast<T*>(std::exchange(other.ptr_, nullptr)))
	        , block_(std::exchange(other.block_, nullptr)) {}

	~unique_ref() { release(); }

	void release() {
		if (!block_) {
			return;
		}

		block_->releaser(block_->ptr);
		block_->ptr = nullptr;

		delete block_;

		ptr_   = nullptr;
		block_ = nullptr;
	}

	[[nodiscard]] T* get() const { return ptr_; }
	[[nodiscard]] T* operator->() const { return get(); }
	[[nodiscard]] T& operator*() const { return *get(); }

	explicit operator bool() const { return get() != nullptr; }

	unique_ref& operator=(unique_ref const&) = delete;

	unique_ref& operator=(unique_ref&& other) {
		if (this != &other) {
			return move_(other);
		}
		return *this;
	}

	template <std::derived_from<T> Other>
	unique_ref& operator=(unique_ref<Other>&& other) {
		return move_(other);
	}

	[[nodiscard]] bool operator==(std::nullptr_t) const { return get() == nullptr; }
	[[nodiscard]] bool operator!=(std::nullptr_t) const { return !(*this == nullptr); }

	template <std::derived_from<T> Other>
	[[nodiscard]] bool operator==(unique_ref<Other> const& other) const {
		return get() == other.get();
	}

	template <std::derived_from<T> Other>
	[[nodiscard]] bool operator!=(unique_ref<Other> const& other) const {
		return !(*this == other);
	}

private:
	explicit unique_ref(T* ptr) {
		block_ = new (std::nothrow) _ref_impl::unique_block_;

		if (!block_) {
			return;
		}

		ptr_             = ptr;
		block_->ptr      = ptr;
		block_->releaser = [](void* p) {
			delete static_cast<T*>(p);
		};
	}

	template <std::derived_from<T> Other_>
	unique_ref& move_(unique_ref<Other_>& other) {
		release();
		ptr_   = static_cast<T*>(std::exchange(other.ptr_, nullptr));
		block_ = std::exchange(other.block_, nullptr);
		return *this;
	}

	T* ptr_;
	_ref_impl::unique_block_* block_;
};

} // namespace aether