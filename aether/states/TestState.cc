#include <aether/objects/Sprite.hh>
#include <aether/states/TestState.hh>
#include <aether/systems/Renderer.hh>

TestState::TestState(ae::Context const& ctx) : ae::Node(ctx) {}
TestState::~TestState() = default;

// protected
bool TestState::init() {
	ctx_.renderer.set_background_rgba({202, 255, 77, 255});

	auto img = ae::Node::create<ae::Sprite>(ctx_, "resources/aether/ui/blackbox.png");
	if (!img) {
		return false;
	}

	img->set_position({400.f, 300.f});
	add(img);

	return true;
}