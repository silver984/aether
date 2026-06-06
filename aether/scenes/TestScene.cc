#include <Context.hh>
#include <cmath>
#ifdef AETHER_DEBUG
	#include <log.hh>
#endif
#include <objects/AnimatedSprite.hh>
#include <objects/TileMap.hh>
#include <objects/abstract/Sound.hh>
#include <scenes/TestScene.hh>
#include <services/core/Renderer.hh>
#include <services/core/Window.hh>

using namespace aether;

TestScene::TestScene(Context const& ctx)
    : Scene(ctx)
    , elapsed_(0.f) {}

TestScene::~TestScene() = default;

// protected
bool TestScene::init() {
	auto const& lctx = ctx();

	if (auto animation = Node::create<AnimatedSprite>(
	        lctx,
	        AnimatedSprite::descriptor{.image_file = "resources/bf.png", .data_file = "resources/bf.xml", .fps = 12})) {
		animation->play_animation("idle", {.loop = true});
		animation->set_position(vec2<float>(0.f, 400.f));
		animation->set_scroll_factor(vec2<float>(0.6f, 1.f));
		animation->set_color(rgba(128, 128, 128, 255));
		add(animation);
	}

	if (auto animation = Node::create<AnimatedSprite>(
	        lctx,
	        AnimatedSprite::descriptor{.image_file = "resources/bf.png", .data_file = "resources/bf.xml", .fps = 12})) {
		animation->play_animation("idle", {.loop = true});
		animation->set_position(vec2<float>(0.f, 400.f));
		add(animation);
	}

	if (auto sound = Sound::create(lctx, "resources/sound.ogg")) {
		add(sound);
		(void)sound->play();
	}

	lctx.core().renderer().set_background_rgba(rgba(128, 128, 128, 255));
	activate();

	return true;
}

// protected
void TestScene::update(float dt) {
	elapsed_ += dt;
	auto& lcamera = camera();
	lcamera.set_position(vec2<float>(lcamera.position().x + (100.f * dt), 0.f));
}