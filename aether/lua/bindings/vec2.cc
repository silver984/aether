#include <debug/log.hh>
#include <lua/binding.hh>
#include <math/vec2.hh>

namespace aether::lua {

struct vec2 : public binding<vec2> {
	static void bind(sol::state_view& lua) {
		using vec2f = aether::vec2<float>;
		lua.new_usertype<vec2f>(
			"vec2f",
			sol::constructors<vec2f(), vec2f(float), vec2f(float, float)>(),
			"x", &vec2f::x,
			"y", &vec2f::y
		);
	}
};

} // namespace aether::lua

namespace { aether::lua::vec2 const _; }