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
    : Scene(ctx) {}

TestScene::~TestScene() = default;

// protected
bool TestScene::init() {
	auto const& lctx = ctx();

	if (auto animation =
	        Node::create<AnimatedSprite>(lctx, AnimatedSprite::descriptor{.image_file = "resources/spooky_dark.png",
	                                                                      .data_file  = "resources/spooky_dark.xml",
	                                                                      .fps        = 24})) {
		animation->play_animation("spooky dance idle", {.loop = true});
		animation->set_position(vec2<float>(600.f, 400.f));
		animation->set_scroll_factor(vec2<float>(1.33f, 0.5f));
		add(animation);
	}

	if (auto animation =
	        Node::create<AnimatedSprite>(lctx, AnimatedSprite::descriptor{.image_file = "resources/spooky_dark.png",
	                                                                      .data_file  = "resources/spooky_dark.xml",
	                                                                      .fps        = 24})) {
		animation->play_animation("spooky dance idle", {.loop = true});
		animation->set_position(vec2<float>(600.f, 400.f));
		add(animation);
	}

	if (auto sound = Sound::create(lctx, "resources/sound.ogg")) {
		add(sound);
		(void)sound->play();
	}

	lctx.core().renderer().set_background_rgba(rgba(128, 128, 128, 255));

	return true;
}