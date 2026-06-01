// #include <aether/objects/PopupUI.hh>
// #include <aether/objects/Sprite.hh>
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
	if (auto animation1 = ae::Node::create<ae::AnimatedSprite>(
	        ctx_, ae::AnimatedSprite::descriptor{.image_file = "resources/cuttinDeezeBalls.png",
	                                             .data_file  = "resources/cuttinDeezeBalls.xml",
	                                             .fps        = 24})) {
		animation1->set_scale(0.6f);
		animation1->play_anim("Whitty Ballistic Cutscene");
		animation1->set_position(ctx_.window.screen_size() * 0.5f);
		add_child(animation1);
	}

	if (auto sound = ae::Sound::create(ctx_, "resources/Inst.mp3")) {
		sound->play();
	}

	ctx_.renderer.set_background_rgba({128, 128, 128, 255});
	return true;
}