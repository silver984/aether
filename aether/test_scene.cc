#include <context.hh>
#include <debug/log.hh>
#include <test_scene.hh>
#include <window.hh>

using namespace aether;

test_scene::test_scene(context const& ctx)
        : scene(ctx)
        , elapsed_(0.f)
        , accumulator_(0) {}

test_scene::~test_scene() = default;

bool test_scene::init_() {
	auto const& ctx  = ctx_();
	auto window_size = ctx.core().fetch_window().target_size();

	og_bf_ = node::create<animated_sprite>(
	        ctx,
	        animated_sprite::descriptor{.image_file = "resources/BOYFRIEND.png", .data_file = "resources/BOYFRIEND.xml", .fps = 24});

	if (!og_bf_) {
		return false;
	}

	og_bf_->play_animation("BF idle dance", {.loop = true});
	og_bf_->set_scale(0.7f);
	og_bf_->set_position(vec2<float>(window_size.width - 400.f, window_size.height / 2.f));
	add(og_bf_);

	bf_spin_ = node::create<animated_sprite>(
	        ctx, animated_sprite::descriptor{.image_file = "resources/bf.png", .data_file = "resources/bf.xml", .fps = 12});

	if (!bf_spin_) {
		return false;
	}

	bf_spin_->play_animation("idle", {.loop = true});
	bf_spin_->set_scale(0.7f);
	bf_spin_->toggle_flip_x(true);
	bf_spin_->set_position(vec2<float>(400.f, window_size.height / 2.f));
	add(bf_spin_);

	death_sound_ = sound::create(ctx, "resources/fnf_loss_sfx.ogg");

	if (!death_sound_) {
		return false;
	}

	activate();

	return true;
}

void test_scene::update_(float dt) {
	if (!bf_spin_ || !og_bf_) {
		return;
	}

	elapsed_ += dt;
	bf_spin_->set_skew(bf_spin_->skew() - (dt * 50.f));

	while (elapsed_ > 3.f) {
		if (++accumulator_ % 2 == 0) {
			og_bf_->play_animation("BF Dead Loop");
		} else {
			og_bf_->play_animation("BF dies");
			death_sound_->play();
		}

		elapsed_ -= 3.f;
	}
}