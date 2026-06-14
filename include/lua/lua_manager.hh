#pragma once
#include <functional>
#include <sol/state_view.hpp>

namespace aether {

class game;

class lua_manager final {
	friend class game;
	template <typename>
	friend class lua_binding;

public:
	~lua_manager();

private:
	lua_manager();
	static std::vector<std::function<void(sol::state_view&)>>& queued_bindings();
	static void queue_binding_(std::function<void(sol::state_view&)>&& cb);
	void run_and_clear_all_bindings_(sol::state_view lua);
	void try_create_scripts_directory_();
	void run_scripts_();
};

} // namespace aether