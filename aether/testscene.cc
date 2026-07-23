#include <context.hh>
#include <debug/log.hh>
#include <lua/manager.hh>
#include <lua/templates.hh>
#include <nodes/animated_sprite.hh>
#include <testscene.hh>
#include <window.hh>

using namespace aether;

testscene::testscene(context const& ctx)
        : scene(ctx) {
}

testscene::~testscene() = default;

bool testscene::init_() {
	if (!scene::init_()) {
		return false;
	}
	return lua::hookchain(this, "testscene:init_", &testscene::init_impl_);
}

bool testscene::init_impl_() {
	auto const& ctx  = ctx_();
	auto window_size = ctx.get_window().target_size();
	auto animated    = node::create<animated_sprite>(
	        ctx, animated_sprite::descriptor{.imagefile = "resources/bf.png", .datafile = "resources/bf.xml", .fps = 12});

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
	lua::hookchain(this, "testscene:update_", &testscene::update_impl_, dt);
}

void testscene::update_impl_(float dt) {
	for (auto& child : root()->children()) {
		child->set_rotation(child->rotation() + (90.f * dt));
	}
}