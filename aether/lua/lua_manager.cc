#include <debug/log.hh>
#include <filesystem>
#include <lua/lua_manager.hh>
#include <util/timer.hh>
#include <utility>

namespace aether {

lua_manager::lua_manager()  = default;
lua_manager::~lua_manager() = default;

std::vector<std::function<void(sol::state_view&)>>& lua_manager::queued_bindings() {
	static std::vector<std::function<void(sol::state_view&)>> instance;
	return instance;
}

void lua_manager::queue_binding_(std::function<void(sol::state_view&)>&& cb) {
	queued_bindings().emplace_back(std::move(cb));
}

void lua_manager::run_and_clear_all_bindings_(sol::state_view lua) {
	auto& bindings = queued_bindings();

	AETHER_DEBUGLOG("Running all bindings | count: {}", bindings.size());
	auto const start_time = util::start();

	for (auto const& cb : bindings) {
		cb(lua);
	}

	auto const end_time = util::end(start_time);
	AETHER_DEBUGLOG("Done | took {}ms", end_time);

	bindings.clear();
}

void lua_manager::try_create_scripts_directory_() {
	(void)std::filesystem::create_directory("scripts");
}

void lua_manager::run_scripts_() {
	std::vector<std::filesystem::path> found_scripts;

	for (auto const& entry : std::filesystem::directory_iterator("scripts")) {
		if (entry.path().extension() == ".lua") {
			found_scripts.emplace_back(entry.path());
		}
	}
}

} // namespace aether