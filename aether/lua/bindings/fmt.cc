#include <fmt/args.h>
#include <fmt/format.h>
#include <lua/binding.hh>
#include <string>
#include <string_view>

namespace {

struct fmt_binding : aether::lua::binding {
	void bind(sol::state_view lua) {
		sol::table fmt_table = lua.create_table();
		fmt_table["format"]  = [](std::string_view str, sol::variadic_args va) -> std::string {
			fmt::dynamic_format_arg_store<fmt::format_context> store;
			for (auto arg : va) {
				switch (arg.get_type()) {
					using enum sol::type;
				case string: {
					store.push_back(arg.as<std::string>());
					break;
				}
				case number: {
					store.push_back(arg.as<float>());
					break;
				}
				case boolean: {
					store.push_back(arg.as<bool>());
					break;
				}
				default: {
					store.push_back(sol::stack::get<std::string>(arg.lua_state(), arg.stack_index()));
					break;
				}
				}
			}
			return fmt::vformat(str, store);
		};
		lua["fmt"] = fmt_table;
	}
};

fmt_binding const* _ = new fmt_binding();

} // namespace