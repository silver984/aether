#include <cmath>
#include <context.hh>
#ifdef AETHER_DEBUG
	#include <log.hh>
#endif
#include <abstract/sound.hh>
#include <objects/animated_sprite.hh>
#include <objects/tilemap.hh>
#include <scenes/test_scene.hh>
#include <services/core/renderer.hh>
#include <services/core/window.hh>

using namespace aether;

test_scene::test_scene(context const& ctx)
    : scene(ctx)
    , elapsed_(0.f) {}

test_scene::~test_scene() = default;

// protected
bool test_scene::init() {
	auto const& lctx = ctx();

	if (auto animation = node::create<animated_sprite>(
	        lctx, animated_sprite::descriptor{
	                  .image_file = "resources/bf.png", .data_file = "resources/bf.xml", .fps = 12})) {
		animation->play_animation("idle", {.loop = true});
		animation->set_position(vec2<float>(0.f, 400.f));
		animation->set_scroll_factor(vec2<float>(0.6f, 1.f));
		animation->set_color(rgba(128, 128, 128, 255));
		add(animation);
	}

	if (auto animation = node::create<animated_sprite>(
	        lctx, animated_sprite::descriptor{
	                  .image_file = "resources/bf.png", .data_file = "resources/bf.xml", .fps = 12})) {
		animation->play_animation("idle", {.loop = true});
		animation->set_position(vec2<float>(0.f, 400.f));
		add(animation);
	}

	if (auto sound = sound::create(lctx, "resources/sound.ogg")) {
		add(sound);
		(void)sound->play();
	}

	lctx.core().fetch_renderer().set_background_rgba(rgba(128, 128, 128, 255));
	activate();

	return true;
}

// protected
void test_scene::update(float dt) {
	elapsed_ += dt;
	auto& lcamera = fetch_camera();
	lcamera.set_position(vec2<float>(lcamera.position().x + (100.f * dt), 0.f));
}