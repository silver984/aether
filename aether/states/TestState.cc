#include <aether/states/TestState.hh>
#include <aether/systems/Renderer.hh>

TestState::TestState(ae::Context const& ctx) : ae::Node(ctx) {}
TestState::~TestState() = default;

// protected
bool TestState::init() {
	if (auto renderer = context().renderer_wref().lock()) {
		renderer->set_background_rgba({202, 255, 77, 255});
	}

	return true;
}