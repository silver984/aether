#include <debug/log.hh>
#include <lua/lua_manager.hh>
#include <sol/environment.hpp>
#include <util/timer.hh>
#include <utility>

namespace aether {

lua_manager::lua_manager()  = default;
lua_manager::~lua_manager() = default;

void lua_manager::init_() {
	using enum sol::lib;
	lua_.open_libraries(base, string, table, math, utf8);
	run_and_clear_all_bindings_();
	try_create_scripts_directory_();
}

std::vector<std::function<void(sol::state_view)>>& lua_manager::queued_bindings_() {
	static std::vector<std::function<void(sol::state_view)>> instance;
	return instance;
}

void lua_manager::queue_binding_(std::function<void(sol::state_view)>&& cb) {
	queued_bindings_().emplace_back(std::move(cb));
}

void lua_manager::run_and_clear_all_bindings_() {
	auto& bindings = queued_bindings_();

	AETHER_DEBUGLOG("Running all bindings | count: {}", bindings.size());
	util::timer t;
	t.start();

	for (auto const& cb : bindings) {
		cb(lua_);
	}

	t.stop();
	AETHER_DEBUGLOG("Done | took {}ms", t.duration());

	bindings.clear();
}

void lua_manager::try_create_scripts_directory_() {
	if (std::filesystem::create_directory("scripts")) {
		AETHER_DEBUGLOG("Created missing lua scripts directory");
	}
}

void lua_manager::run_scripts_() {
	AETHER_INFOLOG("Running lua scripts");
	util::timer t;
	t.start();

	std::vector<std::filesystem::path> available_scripts = gather_available_scripts_();

	if (available_scripts.empty()) {
		AETHER_INFOLOG("Nothing to run");
		return;
	}

	for (auto iterator = available_scripts.begin(); iterator != available_scripts.end();) {
		sol::environment environment                 = sol::environment(lua_, sol::create, lua_.globals());
		sol::protected_function_result script_result = lua_.safe_script_file(
		        iterator->string(), environment,
		        [](lua_State*, sol::protected_function_result result) {
			        sol::error e = result;
			        AETHER_ERRORLOG("Script invalid | what: {}", e.what());
			        return result;
		        },
		        sol::load_mode::any);

		if (!script_result.valid()) {
			AETHER_ERRORLOG("Excluding invalid script | file: \"{}\"", iterator->filename().generic_string());
			iterator = available_scripts.erase(iterator);
			continue;
		}

		++iterator;
	}

	t.stop();
	AETHER_INFOLOG("Done | took {}ms", t.duration());
}

std::vector<std::filesystem::path> lua_manager::gather_available_scripts_() {
	std::vector<std::filesystem::path> out;

	AETHER_DEBUGLOG("Gathering available lua scripts");
	util::timer t;
	t.start();

	for (auto const& entry : std::filesystem::directory_iterator("scripts")) {
		if (entry.path().extension() == ".lua") {
			auto const path = entry.path();
			AETHER_TRACELOG("Found lua script: \"{}\"", path.filename().generic_string());
			out.emplace_back(path);
		}
	}

	t.stop();
	AETHER_DEBUGLOG("Done | count: {} | took {}ms", out.size(), t.duration());

	return out;
}

} // namespace aether