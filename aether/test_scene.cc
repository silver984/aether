#include <cmath>
#include <context.hh>
#ifdef AETHER_DEBUG
	#include <debug/log.hh>
#endif
#include <nodes/animated_sprite.hh>
#include <nodes/tilemap.hh>
#include <renderer.hh>
#include <sound.hh>
#include <test_scene.hh>
#include <window.hh>

using namespace aether;

test_scene::test_scene(context const& ctx)
        : scene(ctx)
        , elapsed_(0.f) {}

test_scene::~test_scene() = default;

// protected
bool test_scene::init_() {
	auto const& lctx = ctx_();

	// if (auto animation = node::create<animated_sprite>(
	//             lctx, animated_sprite::descriptor{.image_file = "resources/bf.png", .data_file = "resources/bf.xml", .fps = 12})) {
	// 	animation->play_animation("idle", {.loop = true});
	// 	animation->set_position(vec2<float>(0.f, 400.f));
	// 	animation->set_scroll_factor(vec2<float>(0.6f, 1.f));
	// 	animation->set_color(rgba(128, 128, 128, 255));
	// 	add(animation);
	// }

	if (auto animation = node::create<animated_sprite>(
	            lctx, animated_sprite::descriptor{.image_file = "resources/bf.png", .data_file = "resources/bf.xml", .fps = 12})) {
		animation->play_animation("idle", {.loop = true});
		animation->set_position(vec2<float>(0.f, 400.f));
		add(animation);
	}

	if (auto sound = sound::create(lctx, "resources/sound.ogg")) {
		add(sound);
		(void)sound->play();
	}

	activate();

	return true;
}

// protected
void test_scene::update_(float dt) {
	elapsed_ += dt;
	auto& lcamera = get_camera();
	lcamera.set_position(vec2<float>(lcamera.position().x + (100.f * dt), 0.f));
}