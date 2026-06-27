#include <debug/log.hh>
#include <lua/binding.hh>
#include <lua/manager.hh>
#include <sol/environment.hpp>
#include <util/timer.hh>
#include <utility>

namespace aether::lua {

manager::manager()  = default;
manager::~manager() = default;

util::string_map<hook>& manager::hook_map() {
	static util::string_map<hook> instance;
	return instance;
}

void manager::try_register_hook(std::string_view function_name, sol::function& callback) {
	auto& hookable_functions = hookable_functions_();
	if (hookable_functions.find(function_name) == hookable_functions.end()) {
		AETHER_ERRORLOG("\"{}\" is not a hookable function", function_name);
		return;
	}
	hook_map()[std::string(function_name)].callbacks.emplace_back(std::move(callback));
	AETHER_TRACELOG("Registered function hook for \"{}\"", function_name);
}

util::string_set const& manager::hookable_functions_() {
	static util::string_set const instance({"testscene:update_"});
	return instance;
}

std::vector<binding*>*& manager::registered_bindings_() {
	static auto* instance = new std::vector<binding*>();
	return instance;
}

void manager::register_binding_(binding* ptr) {
	registered_bindings_()->emplace_back(ptr);
}

void manager::init_() {
	using enum sol::lib;
	state_.open_libraries(base, string, table, math, utf8);
	run_and_clear_all_bindings_();
	try_create_scripts_directory_();
	AETHER_INFOLOG("Initialized");
}

void manager::shutdown_() {
	hook_map().clear();
}

void manager::run_and_clear_all_bindings_() {
	auto& registered_bindings = registered_bindings_();

	AETHER_DEBUGLOG("Running all bindings | count: {}", registered_bindings->size());
	util::timer t;
	t.start();

	for (auto iterator = registered_bindings->begin(); iterator != registered_bindings->end();) {
		(*iterator)->bind(state_);
		delete *iterator;
		iterator = registered_bindings->erase(iterator);
	}

	delete registered_bindings;
	t.stop();
	AETHER_DEBUGLOG("Done | took {}ms", t.duration());
}

void manager::try_create_scripts_directory_() {
	if (std::filesystem::create_directory("scripts")) {
		AETHER_DEBUGLOG("Created missing lua scripts directory");
	}
}

void manager::run_scripts_() {
	AETHER_INFOLOG("Running lua scripts");
	std::vector<std::filesystem::path> available_scripts = gather_available_scripts_();

	if (available_scripts.empty()) {
		AETHER_INFOLOG("Nothing to run");
		return;
	}

	util::timer t;
	t.start();

	for (auto iterator = available_scripts.begin(); iterator != available_scripts.end();) {
		sol::protected_function_result script_result = state_.safe_script_file(
		        iterator->string(), sol::environment(state_, sol::create, state_.globals()),
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

std::vector<std::filesystem::path> manager::gather_available_scripts_() {
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

} // namespace aether::lua