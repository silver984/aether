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

		if (auto renderer = ctx.renderer().lock()) {
			renderer->set_background_rgba(renderer->background_rgba().first, 1.f);
		}

		return true;
	}

	return false;
}

void TestState::update(Context const& ctx, float dt) {
	if (elapsed_ += dt; elapsed_ >= 2.f) {
		if (auto director = ctx.director().lock()) {
			director->switch_state(Node::create<TestState2>(ctx));
		}
	}
}

}