#include <lua/binding.hh>
#include <lua/manager.hh>
#include <string_view>

namespace {

struct ae : aether::lua::binding {
	void bind(sol::state_view lua) override {
		sol::table aether_table = lua.create_table();
		aether_table["hook"]    = [](std::string_view function_name, sol::function callback) -> void {
			aether::lua::manager::try_register_hook(function_name, callback);
		};
		lua["aether"] = aether_table;
	}
};

ae const* _ = new ae();

} // namespace