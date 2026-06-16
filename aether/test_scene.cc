#include <context.hh>
#include <debug/log.hh>
#include <nodes/animated_sprite.hh>
#include <test_scene.hh>
#include <window.hh>

using namespace aether;

test_scene::test_scene(context const& ctx)
        : scene(ctx) {}

test_scene::~test_scene() = default;

bool test_scene::init_() {
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

	// activate();

	return true;
}

// void test_scene::update_(float dt) {
// 	if (!bf_spin_ || !og_bf_) {
// 		return;
// 	}

// 	elapsed_ += dt;
// 	bf_spin_->set_skew(bf_spin_->skew() - (dt * 50.f));

// 	while (elapsed_ > 3.f) {
// 		if (++accumulator_ % 2 == 0) {
// 			og_bf_->play_animation("BF Dead Loop");
// 		} else {
// 			og_bf_->play_animation("BF dies");
// 			death_sound_->play();
// 		}

// 		elapsed_ -= 3.f;
// 	}
// }