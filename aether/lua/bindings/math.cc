#include <algorithm>
#include <fmt/format.h>
#include <functional>
#include <lua/binding.hh>
#include <math/rect.hh>
#include <math/rgba.hh>
#include <math/size.hh>
#include <math/vec2.hh>

namespace {

template <aether::indexed_numeric T, typename U>
void binary(sol::usertype<T>& ut, sol::meta_function meta, U&& op) {
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
}

template <aether::indexed_numeric T>
void operators(sol::usertype<T>& ut) {
	using enum sol::meta_function;

	binary(ut, addition, std::plus<>{});
	binary(ut, subtraction, std::minus<>{});
	binary(ut, multiplication, std::multiplies<>{});
	binary(ut, division, std::divides<>{});

	// todo: to_string

	ut[unary_minus] = [](T obj) -> T {
		return -obj;
	};
	ut[equal_to] = [](T left, T right) -> bool {
		return left == right;
	};
	ut[index] = [](T obj, int index) -> float {
		return obj[(size_t)index - 1];
	};
	ut[new_index] = [](T* self, int index, float value) -> void {
		(*self)[(size_t)index - 1] = value;
	};
	ut["capacity"] = []() -> int {
		return (int)T::capacity();
	};
}

struct math_binding : aether::lua::binding {
	void bind(sol::state_view lua) override {
		{
			using vec2             = aether::vec2<float>;
			using constructors     = sol::constructors<vec2(), vec2(float), vec2(float, float)>;
			sol::usertype<vec2> ut = lua.new_usertype<vec2>("vec2", constructors());
			operators(ut);
			ut["x"] = &vec2::x;
			ut["y"] = &vec2::y;
		}

		{
			using size             = aether::size<float>;
			using constructors     = sol::constructors<size(), size(float), size(float, float)>;
			sol::usertype<size> ut = lua.new_usertype<size>("size", constructors());
			operators(ut);
			ut["width"]  = &size::width;
			ut["height"] = &size::height;
		}

		{
			using rect             = aether::rect<float>;
			using constructors     = sol::constructors<rect(), rect(float), rect(float, float, float, float)>;
			sol::usertype<rect> ut = lua.new_usertype<rect>("rect", constructors());
			operators(ut);
			ut["x"]      = &rect::x;
			ut["y"]      = &rect::y;
			ut["width"]  = &rect::width;
			ut["height"] = &rect::height;
		}

		{
			auto clamp8 = [](int v) -> uint8_t {
				return (uint8_t)std::clamp(v, 0, 255);
			};

			auto constructors = sol::factories(
			        [=](int v) -> aether::rgba {
				        return aether::rgba(clamp8(v));
			        },
			        [=](int r, int g, int b, int a) -> aether::rgba {
				        return aether::rgba(clamp8(r), clamp8(g), clamp8(b), clamp8(a));
			        });

			sol::usertype<aether::rgba> ut = lua.new_usertype<aether::rgba>("rgba", constructors);

			// todo: operators

			ut["r"] = &aether::rgba::r;
			ut["g"] = &aether::rgba::g;
			ut["b"] = &aether::rgba::b;
			ut["a"] = &aether::rgba::a;
		}
	}
};

math_binding* _ = new math_binding();

} // namespace