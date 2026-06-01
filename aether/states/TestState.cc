#include <aether/objects/AnimatedSprite.hh>
#include <aether/objects/abstract/Sound.hh>
#include <aether/states/TestState.hh>
#include <aether/systems/Context.hh>
#include <aether/systems/Renderer.hh>
#include <aether/systems/Window.hh>

TestState::TestState(ae::Context const& ctx)
    : ae::NodeIdentity<TestState>(ctx) {}

TestState::~TestState() = default;

// protected
bool TestState::init() {
	if (auto animation = ae::Node::create<ae::AnimatedSprite>(
	        ctx_, ae::AnimatedSprite::descriptor{.image_file = "resources/WhittyCrazy.png",
	                                             .data_file  = "resources/WhittyCrazy.xml",
	                                             .fps        = 24})) {
		animation->play_anim("Whitty idle dance", {.loop = true});
		animation->set_position({800.f, 300.f});
		add_child(animation);
	}

	if (auto animation = ae::Node::create<ae::AnimatedSprite>(
	        ctx_, ae::AnimatedSprite::descriptor{.image_file = "resources/spooky_dark.png",
	                                             .data_file  = "resources/spooky_dark.xml",
	                                             .fps        = 24})) {
		animation->play_anim("spooky dance idle", {.loop = true});
		animation->set_position({400.f, 400.f});
		add_child(animation);
	}

	if (auto sound = ae::Sound::create(ctx_, "resources/Inst.mp3")) {
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