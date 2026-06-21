#pragma once
#include <lua/manager.hh>
#include <sol/state_view.hpp>

namespace aether::lua {

struct binding {
	binding() {
		manager::register_binding_(this);
	}
	virtual ~binding()                 = default;
	virtual void bind(sol::state_view) = 0;
};

} // namespace aether