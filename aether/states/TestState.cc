#include <aether/states/TestState.hh>
#include <aether/states/TestState2.hh>
#include <aether/systems/Window.hh>
#include <aether/systems/Renderer.hh>
#include <cmath>

TestState::TestState() = default;
TestState::~TestState() = default;

bool TestState::init(ae::Context const& ctx) {
	if (auto renderer = ctx.renderer().lock()) {
		renderer->set_background_rgba(ae::rgb(202ui8, 255ui8, 77ui8), 1.f);
	}

	if (auto grah = ae::Node::create<ae::Graphic>(ctx, "resources/grah.png")) {
		if (auto window = ctx.window().lock()) {
			grah->set_position((ae::vec2<float>)window->screen_size() / 2.f);
		}

		add(grah);
		
		return true;
	}

	return false;
}

