#include <Context.hh>
#include <objects/AnimatedSprite.hh>
#include <objects/abstract/Sound.hh>
#include <services/Renderer.hh>
#include <services/Window.hh>
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
		animation->set_position({400.f, 400.f});
		add_child(animation);
	}

	if (auto sound = Sound::create(ctx_, "resources/Inst.mp3")) {
		sound->play();
	}

	ctx_.renderer.set_background_rgba({128, 128, 128, 255});
	activate();

	return true;
}

// protected
void TestState::update(float dt) {
	for (auto& child : children()) {
		if (child) {
			child->set_skew(child->skew() + (45.f * dt));
			child->set_rotation(child->rotation() + (90.f * dt));
		}
	}
}