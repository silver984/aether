#pragma once
#include <lua/lua_manager.hh>
#include <sol/state_view.hpp>

namespace aether {

struct lua_binding {
	lua_binding() {
		lua_manager::register_binding_(this);
	}
	virtual ~lua_binding()             = default;
	virtual void bind(sol::state_view) = 0;
};

} // namespace aether