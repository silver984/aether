#include <debug/log.hh>
#include <lua/lua_binding.hh>
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
	AETHER_INFOLOG("Initialized");
}

std::vector<lua_binding*>*& lua_manager::registered_bindings_() {
	static auto* instance = new std::vector<lua_binding*>();
	return instance;
}

void lua_manager::register_binding_(lua_binding* ptr) {
	registered_bindings_()->emplace_back(ptr);
}

void lua_manager::run_and_clear_all_bindings_() {
	auto& registered_bindings = registered_bindings_();

	AETHER_DEBUGLOG("Running all bindings | count: {}", registered_bindings->size());
	util::timer t;
	t.start();

	for (auto iterator = registered_bindings->begin(); iterator != registered_bindings->end();) {
		(*iterator)->bind(lua_);
		delete *iterator;
		iterator = registered_bindings->erase(iterator);
	}

	delete registered_bindings;
	t.stop();
	AETHER_DEBUGLOG("Done | took {}ms", t.duration());
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
		sol::protected_function_result script_result = lua_.safe_script_file(
		        iterator->string(), sol::environment(lua_, sol::create, lua_.globals()),
		        [](lua_State*, sol::protected_function_result result) {
			        sol::error e = result;
			        AETHER_ERRORLOG("Script invalid | what: {}", e.what());
			        return result;
		        },
		        sol::load_mode::any);

		if (!script_result.valid()) {
			AETHER_ERRORLOG("Excluding invalid script | file: \"{}\"", iterator->filename().string());
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