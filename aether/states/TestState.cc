#include <aether/states/TestState.hh>
#include <aether/objects/Sprite.hh>
#include <aether/systems/Window.hh>
#include <aether/systems/Renderer.hh>
#include <cmath>

TestState::TestState() = default;
TestState::~TestState() = default;

bool TestState::init(ae::Context const& ctx) {
	if (auto renderer = ctx.renderer_wref().lock()) {
		renderer->set_background_rgba(ae::rgb(ae::rgb::as_float(202), ae::rgb::as_float(255), ae::rgb::as_float(77)), 1.f);
	}

	if (auto grah = ae::Node::create<ae::Sprite>(ctx, "resources/grah.png")) {
		if (auto window = ctx.window_wref().lock()) {
			grah->set_position(static_cast<ae::vec2<float>>(window->screen_size()) / 2.f);
		}

		add(grah);
		
		return true;
	}

	return false;
}

