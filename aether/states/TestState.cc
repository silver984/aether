#include <aether/objects/AnimatedSprite.hh>
#include <aether/states/TestState.hh>
#include <aether/systems/Renderer.hh>

TestState::TestState(ae::Context const& ctx) : ae::Node(ctx) {}
TestState::~TestState() = default;

// protected
bool TestState::init() {
	ctx_.renderer.set_background_rgba({202, 255, 77, 255});

	auto animation = ae::Node::create<ae::AnimatedSprite>(ctx_, "resources/bf.png", "resources/bf.xml", 12);
	if (!animation) {
		return false;
	}

	animation->play_anim("idle", true);
	animation->set_position({640.f, 360.f});
	add(animation);

	return true;
}