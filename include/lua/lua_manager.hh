#pragma once
#include <filesystem>
#include <sol/state.hpp>
#include <vector>

namespace aether {

class game;
struct lua_binding;

class lua_manager final {
	friend class game;
	friend class lua_binding;

public:
	~lua_manager();

private:
	lua_manager();
	void init_();
	static std::vector<lua_binding*>*& registered_bindings_();
	static void register_binding_(lua_binding* ptr);
	void run_and_clear_all_bindings_();
	void try_create_scripts_directory_();
	void run_scripts_();
	[[nodiscard]] std::vector<std::filesystem::path> gather_available_scripts_();

	sol::state lua_;
};

} // namespace aether