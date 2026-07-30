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

void manager::try_register_hook(std::string_view function_name, sol::function&& callback) {
	auto& hookable_functions = hookable_functions_();
	if (hookable_functions.find(function_name) == hookable_functions.end()) {
		AE_ERRORLOG("\"{}\" is not a hookable function", function_name);
		return;
	}

	auto& lhook_map = hook_map();
	if (lhook_map.size() >= lhook_map.max_size()) {
		AE_ERRORLOG("Can't register any more function hooks!");
		return;
	}

	std::string const function_name_str = std::string(function_name);
	lhook_map[function_name_str].callbacks.emplace_back(std::move(callback));
	AE_TRACELOG("Registered function hook for \"{}\" ? hooked: {}", function_name, lhook_map[function_name_str].callbacks.size());
}

util::string_set const& manager::hookable_functions_() {
	static util::string_set const instance({"testscene:init_", "testscene:update_"});
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
	AE_INFOLOG("Initialized");
}

void manager::shutdown_() {
	hook_map().clear();
}

void manager::run_and_clear_all_bindings_() {
	auto& registered_bindings = registered_bindings_();

	AE_DEBUGLOG("Running all bindings ? count: {}", registered_bindings->size());
	util::timer t;
	t.start();

	for (auto it = registered_bindings->begin(); it != registered_bindings->end();) {
		(*it)->bind(state_);
		delete *it;
		*it = nullptr;
		it  = registered_bindings->erase(it);
	}

	delete registered_bindings;
	registered_bindings = nullptr;

	t.stop();
	AE_DEBUGLOG("Done ({}ms)", t.duration());
}

void manager::try_create_scripts_directory_() {
	if (std::filesystem::create_directory("scripts")) {
		AE_DEBUGLOG("Created missing lua scripts directory");
	}
}

void manager::run_scripts_() {
	AE_INFOLOG("Running lua scripts");
	std::vector<std::filesystem::path> available_scripts = gather_available_scripts_();

	if (available_scripts.empty()) {
		AE_INFOLOG("Nothing to run");
		return;
	}

	util::timer t;
	t.start();

	for (auto it = available_scripts.begin(); it != available_scripts.end();) {
		sol::protected_function_result script_result = state_.safe_script_file(
		        it->string(), sol::environment(state_, sol::create, state_.globals()),
		        [](lua_State*, sol::protected_function_result result) -> sol::protected_function_result {
			        sol::error e = result;
			        AE_ERRORLOG("Script invalid ? what: {}", e.what());
			        return result;
		        },
		        sol::load_mode::any);
		if (!script_result.valid()) {
			AE_ERRORLOG("Excluding invalid script ? file: \"{}\"", it->filename().string());
			it = available_scripts.erase(it);
			continue;
		}
		++it;
	}

	t.stop();
	AE_INFOLOG("Done ({}ms)", t.duration());
}

std::vector<std::filesystem::path> manager::gather_available_scripts_() {
	std::vector<std::filesystem::path> out;

	AE_DEBUGLOG("Gathering available lua scripts");
	util::timer t;
	t.start();

	for (auto const& entry : std::filesystem::directory_iterator("scripts")) {
		if (entry.path().extension() == ".lua") {
			auto const path = entry.path();
			AE_TRACELOG("Found lua script: \"{}\"", path.filename().generic_string());
			out.emplace_back(path);
		}
	}

	t.stop();
	AE_DEBUGLOG("Done ({}ms) ? count: {} ", t.duration(), out.size());

	return out;
}

} // namespace aether::lua