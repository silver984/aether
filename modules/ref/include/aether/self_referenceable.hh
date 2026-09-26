#pragma once

#include "weak_ref.hh"

namespace aether {

template <typename T>
class self_referenceable {
	template <typename>
	friend class strong_ref;

	template <typename>
	friend class weak_ref;

public:
	using _is_self_referenceable = void;

	self_referenceable(self_referenceable const&) {}
	self_referenceable(self_referenceable&&) {}
	self_referenceable& operator=(self_referenceable const&) {}
	self_referenceable& operator=(self_referenceable&&) {}

protected:
	self_referenceable()          = default;
	virtual ~self_referenceable() = default;

	[[nodiscard]] strong_ref<T> strong_self_() const { return weak_.construct(); }
	[[nodiscard]] weak_ref<T> weak_self_() const { return weak_; }

private:
	void init_self_reference_(strong_ref<T> const& val) {
		// its expected that this function is only called once
		weak_ = val;
	}

	weak_ref<T> mutable weak_;
};

} // namespace aether