#pragma once
#include <sol/state_view.hpp>

namespace aether {

class lua_binding {
public:
	lua_binding();
	virtual ~lua_binding();

	void invoke(sol::state_view lua);
};

} // namespace aether