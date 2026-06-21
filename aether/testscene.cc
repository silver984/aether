#include <context.hh>
#include <debug/log.hh>
#include <lua/manager.hh>
#include <nodes/animated_sprite.hh>
#include <testscene.hh>
#include <window.hh>

using namespace aether;

testscene::testscene(context const& ctx)
        : scene(ctx) {
}

testscene::~testscene() = default;

bool testscene::init_() {
	auto const& ctx  = ctx_();
	auto window_size = ctx.get_window().target_size();
	auto animated    = node::create<animated_sprite>(
	        ctx, animated_sprite::descriptor{.image_file = "resources/bf.png", .data_file = "resources/bf.xml", .fps = 12});

	if (!animated) {
		return false;
	}

	animated->play_animation("idle", {.loop = true});
	animated->set_scale(0.7f);
	animated->toggle_flip_x(true);
	animated->set_position(vec2<float>(400.f, window_size.height / 2.f));
	add(animated);
	activate();

	return true;
}

void testscene::update_(float dt) {
	auto& lua_hooks              = lua::manager::hook_map();
	auto const hook_map_iterator = lua_hooks.find("testscene:update_");
	if (hook_map_iterator == lua_hooks.end()) {
		return;
	}

	auto& registered_callbacks = hook_map_iterator->second.callbacks;
	for (auto iterator = registered_callbacks.begin(); iterator != registered_callbacks.end();) {
		sol::protected_function_result function_result = (*iterator)(this, dt);
		if (!function_result.valid()) {
			sol::error e = function_result;
			AETHER_ERRORLOG("Invalid hook callback | what: {}", e.what());
			iterator = registered_callbacks.erase(iterator);
			if (iterator == registered_callbacks.end()) {
				lua_hooks.erase(hook_map_iterator);
			}
			continue;
		}
		++iterator;
	}
}