#include <Context.hh>
#include <cmath>
#include <log.hh>
#include <objects/AnimatedSprite.hh>
#include <objects/TileMap.hh>
#include <objects/abstract/Sound.hh>
#include <services/core/Renderer.hh>
#include <services/core/Window.hh>
#include <states/TestState.hh>

using namespace aether;

TestState::TestState(Context const& ctx)
    : NodeIdentity<TestState>(ctx) {}

TestState::~TestState() = default;

// protected
bool TestState::init() {
	if (auto animation =
	        Node::create<AnimatedSprite>(ctx_, AnimatedSprite::descriptor{.image_file = "resources/spooky_dark.png",
	                                                                      .data_file  = "resources/spooky_dark.xml",
	                                                                      .fps        = 24})) {
		animation->play_animation("spooky dance idle", {.loop = true});
		animation->set_position(vec2<float>(600.f, 400.f));
		add_child(animation);
	}

	if (auto icon = Node::create<TileMap>(
	        ctx_, TileMap::descriptor{.file = "resources/icon-bf-old.png", .tile_bounds = size<int>(150)})) {
		icon->set_position(vec2<float>(800.f, 400.f));
		icon->set_rotation(45.f);
		icon->set_name("icon");
		add_child(icon);
	}

	if (auto sound = Sound::create(ctx_, "resources/Inst.mp3")) {
		sound->play();
	}

	ctx_.core_services.renderer.set_background_rgba(rgba(128, 128, 128, 255));
	activate();

	return true;
}

// protected
void TestState::update(float dt) {
	for (auto& child : children()) {
		if (child) {
			child->set_rotation(child->rotation() + (90.f * dt));
		}
	}

	if (auto node = fetch_child_as<TileMap>("icon")) {
		if ((int)std::round(node->rotation()) % 360 == 0) {
			node->seek_tile(vec2<int>(1, 0));
		}
	}
}