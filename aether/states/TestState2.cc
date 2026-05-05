#include <aether/states/TestState2.hh>
#include <aether/states/TestState.hh>
#include <aether/objects/Graphic.hh>
#include <aether/systems/Director.hh>
#include <aether/systems/Renderer.hh>
#include <fmt/format.h>

namespace ae {

TestState2::TestState2() :
	elapsed_(0.f)
{};

TestState2::~TestState2() = default;

bool TestState2::init(Context const& ctx) {
	if (auto grah = Node::create<Graphic>(ctx, "resources/grah.png")) {
		this->add(grah);
		grah->set_scale(vec2<float>(0.5f, 0.5f));
		this->activate();

		if (auto renderer = ctx.renderer().lock()) {
			renderer->set_background_rgba(renderer->background_rgba().first, 0.f);
		}

		return true;
	}

	return false;
}

void TestState2::update(Context const& ctx, float dt) {
	if (elapsed_ += dt; elapsed_ >= 2.f) {
		if (auto director = ctx.director().lock()) {
			director->switch_state(Node::create<TestState>(ctx));
		}
	}
}

}