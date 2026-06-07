#include <lua/bindings/math/lvec2.hh>
#include <math/vec2.hh>
#include <sol/sol.hpp>

namespace aether::lua {

void lvec2::expose(sol::state_view lua) {
	using vec2i = vec2<float>;
	lua.new_usertype<vec2i>("vec2i", sol::constructors<vec2i(), vec2i(int), vec2i(int, int)>(), "x", &vec2i::x, "y",
	                        &vec2i::y);

	using vec2f = vec2<float>;
	lua.new_usertype<vec2f>("vec2f", sol::constructors<vec2f(), vec2f(float), vec2f(float, float)>(), "x", &vec2f::x,
	                        "y", &vec2f::y);
}

} // namespace aether::lua