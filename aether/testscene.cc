#include <context.hh>
#include <debug/log.hh>
// #include <lua/manager.hh>
// #include <lua/templates.hh>
#include <nodes/sprite.hh>
#include <testscene.hh>
#include <window.hh>

using namespace aether;

testscene::testscene(context const& ctx) noexcept
        : scene(ctx) {
}
testscene::~testscene() noexcept = default;

bool testscene::init_() {
	if (!scene::init_()) {
		return false;
	}

	auto const& ctx  = ctx_();
	auto window_size = ctx.window().target_size();
	auto spr         = node::create<sprite>(ctx, sprite_args{.file = "cat.jpg"});
	if (!spr) {
		return false;
	}

	spr->set_scale(0.4f);
	spr->set_position(window_size / 2.f);
	add(spr);
	activate();

	return true;
}

// bool testscene::init_impl_() {
// 	auto const& ctx  = ctx_();
// 	auto window_size = ctx.get_window().target_size();
// 	auto sprite    = node::create<animated_sprite>(
// 	        ctx, animated_sprite::descriptor{.imagefile = "resources/bf.png", .datafile = "resources/bf.xml", .fps = 12});

// 	if (!sprite) {
// 		return false;
// 	}

// 	sprite->play_animation("idle", {.loop = true});
// 	sprite->set_scale(0.7f);
// 	sprite->toggle_flip_x(true);
// 	sprite->set_position(vec2<float>(400.f, window_size.height / 2.f));
// 	add(sprite);
// 	activate();

// 	return true;
// }

void testscene::update_(float dt) {
	// lua::hookchain(this, "testscene:update_", &testscene::update_impl_, dt);
	for (auto& child : root()->children()) {
		child->set_rotation(child->rotation() + (22.5f * dt));
	}
}

// void testscene::update_impl_(float dt) {
// 	for (auto& child : root()->children()) {
// 		child->set_rotation(child->rotation() + (90.f * dt));
// 	}
// }