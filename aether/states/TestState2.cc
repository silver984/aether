#include <aether/states/TestState2.hh>
#include <aether/states/TestState.hh>
#include <aether/objects/Graphic.hh>
#include <aether/systems/Director.hh>
#include <fmt/format.h>

namespace ae {

TestState2::TestState2() :
	elapsed_(0.f)
{};

TestState2::~TestState2() {
	fmt::print("TestState2 destroyed\n");
}

bool TestState2::init(Context const& ctx) {
	if (auto grah = Node::create<Graphic>(ctx, "resources/grah.png")) {
		this->add(grah);
		grah->set_scale(vec2<float>(0.5f, 0.5f));
		this->activate();
		return true;
	}

	return false;
}

void TestState2::update(Context const& ctx, float dt) {
	if (elapsed_ += dt; elapsed_ >= 4.f) {
		if (auto director = ctx.director().lock()) {
			director->switch_state(Node::create<TestState>(ctx));
		}
	}
}

}