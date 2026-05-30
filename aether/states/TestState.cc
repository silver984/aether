#include <aether/objects/PopupUI.hh>
#include <aether/states/TestState.hh>
#include <aether/systems/Renderer.hh>
#include <aether/systems/Window.hh>

TestState::TestState(ae::Context const& ctx) : ae::NodeIdentity<TestState>(ctx) {}
TestState::~TestState() = default;

// protected
bool TestState::init() {
	// auto bf = ae::Node::create<ae::AnimatedSprite>(ctx_, "resources/bf.png", "resources/bf.xml", 12);

	// if (!bf) {
	// 	return false;
	// }

	// bf->play_anim("idle", true);
	// bf->set_position(ctx_.window.screen_size() / 2.f);
	// add(bf);

	// auto animation1 = ae::Node::create<ae::AnimatedSprite>(ctx_, "resources/cuttinDeezeBalls.png",
	//                                                        "resources/cuttinDeezeBalls.xml", 24);
	// if (!animation1) {
	// 	return false;
	// }

	// animation1->set_scale(0.6f);
	// animation1->play_anim("Whitty Ballistic Cutscene", true);
	// animation1->set_position(ctx_.window.screen_size() / 2.f);
	// add(animation1);

	// auto animation2 =
	//     ae::Node::create<ae::AnimatedSprite>(ctx_, "resources/WhittyCrazy.png", "resources/WhittyCrazy.xml", 24);

	// if (!animation2) {
	// 	return false;
	// }

	// animation2->set_scale(0.8f);
	// animation2->play_anim("Whitty idle dance", true);
	// animation2->set_position(ctx_.window.screen_size() / 2.f);
	// add(animation2);

	if (auto popup = ae::Node::create<ae::PopupUI>(ctx_)) {
		add(popup);
	}

	ctx_.renderer.set_background_rgba({128, 128, 128, 255});

	return true;
}