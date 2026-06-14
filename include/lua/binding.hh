#pragma once
#include <lua/queue.hh>

namespace aether::lua {

template <typename T>
struct binding {
	binding() {
		impl_::queue_binding_([](sol::state_view& lua) {
			T::bind(lua);
		});
	}
};

} // namespace aether::lua