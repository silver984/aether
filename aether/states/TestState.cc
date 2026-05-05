#include <aether/states/TestState.hh>
#include <aether/states/TestState2.hh>
#include <aether/objects/Graphic.hh>
#include <aether/systems/Director.hh>
#include <aether/systems/Renderer.hh>
#include <fmt/format.h>

namespace ae {

TestState::TestState() :
	elapsed_(0.f)
{};

TestState::~TestState() = default;

bool TestState::init(Context const& ctx) {
	if (auto grah = Node::create<Graphic>(ctx, "resources/grah.png")) {
		this->add(grah);
		this->activate();

		return true;
	}

	return false;
}

void TestState::update(Context const& ctx, float dt) {
	elapsed_ += dt;

	float speed = 2.0f;
	float oscillating = 0.5f * (std::sin(speed * elapsed_) + 1.f);

	if (auto renderer = ctx.renderer().lock()) {
		renderer->set_background_rgba(
			renderer->background_rgba().first,
			oscillating
		);
	}
}

}