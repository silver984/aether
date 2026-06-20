#include <algorithm>
#include <lua/lua_binding.hh>
#include <math/rect.hh>
#include <math/size.hh>
#include <math/vec2.hh>

namespace {

template <aether::indexed_numeric T, aether::numeric U>
using ctor2 = sol::constructors<T(), T(U), T(U, U)>;

struct math : aether::lua_binding {
	void bind(sol::state_view lua) override {
		using vec2 = aether::vec2<double>;
		bind_common(lua.new_usertype<vec2>("vec2", ctor2<vec2, double>(), "x", &vec2::x, "y", &vec2::y));
	}

	template <aether::indexed_numeric T>
	void bind_common(sol::usertype<T>&& usertype) {
		using enum sol::meta_function;
#define META(meta, op)                                                                                                                     \
	usertype[meta] = sol::overload(                                                                                                    \
	        [](T const& left, T const& right) -> T {                                                                                   \
		        return left op right;                                                                                              \
	        },                                                                                                                         \
	        [](T const& left, double right) -> T {                                                                                     \
		        return left op right;                                                                                              \
	        })
		META(addition, +);
		META(subtraction, -);
		META(multiplication, *);
		META(division, /);
#undef META
		usertype[equal_to] = [](T const& left, T const& right) -> bool {
			return left == right;
		};
		usertype[index] = [](T const& container, int index) -> double {
			return container[index - 1];
		};
		usertype[new_index] = [](T& container, int index, double value) -> void {
			container[index - 1] = value;
		};
		usertype["capacity"] = []() -> int {
			return (int)T::capacity();
		};
	}
};

math const* _ = new math();

} // namespace