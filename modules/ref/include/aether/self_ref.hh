#pragma once

#include "weak_ref.hh"

namespace aether {

template <typename T>
class self_ref {
	template <typename>
	friend class weak_ref;

	template <typename>
	friend class strong_ref;

public:
	using _self_referenceable = void;

	self_ref(self_ref const&) {}
	self_ref(self_ref&&) {}
	self_ref& operator=(self_ref const&) {}
	self_ref& operator=(self_ref&&) {}

protected:
	self_ref()          = default;
	virtual ~self_ref() = default;

	[[nodiscard]] strong_ref<T> strong_self_() const { return weak_.construct(); }
	[[nodiscard]] weak_ref<T> weak_self_() const { return weak_; }

private:
	void init_self_ref_(strong_ref<T> const& ref) {
		// its expected that this function is only called once
		weak_ = ref;
	}

	weak_ref<T> mutable weak_;
};

} // namespace aether