#pragma once
#include <lua/manager.hh>
#include <sol/state.hpp>

namespace aether::lua {

struct binding {
	binding() {
		manager::register_binding_(this);
	}
	binding(binding const&)            = delete;
	binding(binding&&)                 = delete;
	binding& operator=(binding const&) = delete;
	binding& operator=(binding&&)      = delete;
	virtual ~binding()                 = default;
	virtual void bind(sol::state_view) = 0;
};

} // namespace aether::lua