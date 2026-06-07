#pragma once
#include <lua/binding.hh>

namespace aether::lua {

struct lvec2 final : public binding {
	using binding::binding;
	void expose(sol::state_view lua) override;
};

} // namespace aether