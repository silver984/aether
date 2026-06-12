#include <context.hh>
#include <nodes/animated_sprite.hh>
#include <test_scene.hh>

#include <debug/log.hh>

using namespace aether;

test_scene::test_scene(context const& ctx)
        : scene(ctx)
        , elapsed_(0.f) {}

test_scene::~test_scene() = default;

bool test_scene::init_() {
	auto const& ctx = ctx_();

	auto whitty = node::create<animated_sprite>(ctx, animated_sprite::descriptor{.image_file = "resources/cuttinDeezeBalls.png",
	                                                                             .data_file  = "resources/cuttinDeezeBalls.xml",
	                                                                             .fps        = 24});
	whitty->play_animation("Whitty Ballistic Cutscene", {.loop = true});
	whitty->set_scale(0.6f);
	whitty->set_position(vec2<float>(400.f));
	add(whitty);

	auto bf = node::create<animated_sprite>(
	        ctx, animated_sprite::descriptor{.image_file = "resources/bf.png", .data_file = "resources/bf.xml", .fps = 12});
	bf->play_animation("idle", {.loop = true});
	bf->set_position(vec2<float>(800.f));
	whitty->add_child(bf);

	sound_ = sound::create(ctx, "resources/sound.ogg");
	sound_->play();
	add(sound_);

	// activate();
	return true;
}

void test_scene::update_(float dt) {}