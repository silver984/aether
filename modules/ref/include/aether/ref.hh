#pragma once

#include "self_referenceable.hh"
#include "strong_ref.hh"
#include "unique_ref.hh"
#include "weak_ref.hh"

#include <type_traits>

namespace aether {

struct ref final {
	ref() = delete;

	template <typename T, typename... Args>
	[[nodiscard]] static strong_ref<T> strong(Args&&... args) {
		return strong_ref<T>(new T(std::forward<Args>(args)...));
	}

	template <typename T, typename... Args>
	[[nodiscard]] static unique_ref<T> unique(Args&&... args) {
		return unique_ref<T>(new T(std::forward<Args>(args)...));
	}

	template <typename T, typename Other>
	        requires std::is_base_of_v<Other, T>
	[[nodiscard]] static strong_ref<T> dynamic_strong_cast(strong_ref<Other> const& other) {
		T* ptr = dynamic_cast<T*>(other.ptr_);
		if (!ptr) {
			return nullptr;
		}
		strong_ref<T> out;
		out.ptr_   = ptr;
		out.block_ = other.block_;
		out.increment_strong_count_();
		return out;
	}
};

} // namespace aether