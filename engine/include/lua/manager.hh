#pragma once
#include <filesystem>
#include <sol/sol.hpp>
#include <util/string.hh>
#include <vector>

namespace aether {

class game;

}

namespace aether::lua {

struct binding;

struct hook final {
	std::vector<sol::function> callbacks;
};

class manager final {
	friend class aether::game;
	friend class binding;

public:
	~manager();
	[[nodiscard]] static util::string_map<hook>& hook_map();
	static void try_register_hook(std::string_view function_name, sol::function&& callback);

private:
	manager();
	[[nodiscard]] static util::string_set const& hookable_functions_();
	[[nodiscard]] static std::vector<binding*>*& registered_bindings_();
	static void register_binding_(binding* ptr);
	void init_();
	void shutdown_();
	void run_and_clear_all_bindings_();
	void try_create_scripts_directory_();
	void run_scripts_();
	[[nodiscard]] std::vector<std::filesystem::path> gather_available_scripts_();

	sol::state state_;
};

} // namespace aether::lua