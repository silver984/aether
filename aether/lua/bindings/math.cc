#include <algorithm>
#include <functional>
#include <lua/lua_binding.hh>
#include <math/vec2.hh>

namespace {

template <aether::indexed_numeric T, typename U>
sol::usertype<T>& binary(sol::usertype<T>& ut, sol::meta_function meta, U&& op) {
	ut[meta] = sol::overload(
	        [=](T left, T right) -> T {
		        return op(left, right);
	        },
	        [=](T left, float right) -> T {
		        return op(left, right);
	        },
	        [=](float left, T right) -> T {
		        return op(right, left);
	        });
	return ut;
}

template <aether::indexed_numeric T>
sol::usertype<T>& operators(sol::usertype<T>& ut) {
	using enum sol::meta_function;

	ut = binary(ut, addition, std::plus<>{});
	ut = binary(ut, subtraction, std::minus<>{});
	ut = binary(ut, multiplication, std::multiplies<>{});
	ut = binary(ut, division, std::divides<>{});

	ut[equal_to] = [](T left, T right) -> bool {
		return left == right;
	};
	ut[index] = [](T obj, int index) -> float {
		return obj[index - 1];
	};
	ut[new_index] = [](T& self, int index, float value) -> void {
		self[index - 1] = value;
	};
	ut["capacity"] = []() -> int {
		return (int)T::capacity();
	};

	return ut;
}

struct math : aether::lua_binding {
	void bind(sol::state_view lua) override {
		{
			using vec2             = aether::vec2<float>;
			using constructors     = sol::constructors<vec2(), vec2(float), vec2(float, float)>;
			sol::usertype<vec2> ut = lua.new_usertype<vec2>("vec2", constructors());
			ut                     = operators(ut);
			ut["x"]                = &vec2::x;
			ut["y"]                = &vec2::y;
		}
	}
};

math const* _ = new math();

} // namespace