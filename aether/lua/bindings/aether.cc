#include <lua/binding.hh>
#include <lua/manager.hh>
#include <string_view>
#include <utility>

namespace {

struct aether_binding : aether::lua::binding {
	void bind(sol::state_view lua) override {
		sol::table aether_table = lua.create_table();
		aether_table["hook"]    = [](std::string_view function_name, sol::function callback) -> void {
			aether::lua::manager::try_register_hook(function_name, std::move(callback));
		};
		lua["aether"] = aether_table;
	}
};

aether_binding const* _ = new aether_binding();

} // namespace