#pragma once
#include <lua/lua_manager.hh>

namespace aether {

template <typename T>
struct lua_binding {
	lua_binding() {
		lua_manager::queue_binding_([](sol::state_view& lua) -> void {
			T::bind(lua);
		});
	}
};

} // namespace aether