#include <debug/log.hh>
#include <lua/lua_manager.hh>
#include <util/timer.hh>
#include <utility>

namespace aether {

lua_manager::lua_manager()  = default;
lua_manager::~lua_manager() = default;

std::vector<std::function<void(sol::state_view&)>>& lua_manager::queued_bindings_() {
	static std::vector<std::function<void(sol::state_view&)>> instance;
	return instance;
}

void lua_manager::queue_binding_(std::function<void(sol::state_view&)>&& cb) {
	queued_bindings_().emplace_back(std::move(cb));
}

void lua_manager::run_and_clear_all_bindings_(sol::state_view lua) {
	auto& bindings = queued_bindings_();

	AETHER_DEBUGLOG("Running all bindings | count: {}", bindings.size());
	util::timer t;
	t.start();

	for (auto const& cb : bindings) {
		cb(lua);
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
	std::vector<std::filesystem::path> available_scripts = gather_available_scripts_();
	if (available_scripts.empty()) {
		AETHER_DEBUGLOG("No lua scripts to run");
		return;
	}
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