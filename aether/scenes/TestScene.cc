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
#include <soloud.h>
#include <soloud_wav.h>

using namespace aether;

TestScene::TestScene(Context const& ctx)
    : Scene(ctx) {}

TestScene::~TestScene() = default;

// protected
bool TestScene::init() {
	if (auto animation =
	        Node::create<AnimatedSprite>(ctx_, AnimatedSprite::descriptor{.image_file = "resources/spooky_dark.png",
	                                                                      .data_file  = "resources/spooky_dark.xml",
	                                                                      .fps        = 24})) {
		animation->play_animation("spooky dance idle", {.loop = true});
		animation->set_position(vec2<float>(600.f, 400.f));
		add(animation);
	}

	if (auto icon = Node::create<TileMap>(
	        ctx_, TileMap::descriptor{.file = "resources/icon-bf-old.png", .tile_bounds = size<int>(150)})) {
		icon->set_position(vec2<float>(800.f, 400.f));
		icon->set_name("icon");
		add(icon);
	}

	if (auto animation = Node::create<AnimatedSprite>(
	        ctx_, AnimatedSprite::descriptor{.image_file = "resources/cuttinDeezeBalls.png",
	                                         .data_file  = "resources/cuttinDeezeBalls.xml",
	                                         .fps        = 24})) {
		animation->play_animation("Whitty Ballistic Cutscene", {.loop = true});
		animation->set_scale(0.8f);
		animation->set_position(vec2<float>(400.f));
		add(animation);
	}

	return true;
}