#pragma once
#include <filesystem>
#include <functional>
#include <sol/state.hpp>
#include <vector>

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
	void init_();
	static std::vector<std::function<void(sol::state_view)>>& queued_bindings_();
	static void queue_binding_(std::function<void(sol::state_view)>&& cb);
	void run_and_clear_all_bindings_();
	void try_create_scripts_directory_();
	void run_scripts_();
	[[nodiscard]] std::vector<std::filesystem::path> gather_available_scripts_();

	sol::state lua_;
};

} // namespace aether