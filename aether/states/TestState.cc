#include <aether/states/TestState.hh>
#include <aether/states/TestState2.hh>
#include <aether/objects/Graphic.hh>
#include <aether/systems/Director.hh>
#include <fmt/format.h>

namespace ae {

TestState::TestState() :
	elapsed_(0.f)
{};

TestState::~TestState() {
	fmt::print("TestState destroyed\n");
}

bool TestState::init(Context const& ctx) {
	if (auto grah = Node::create<Graphic>(ctx, "resources/grah.png")) {
		this->add(grah);
		this->activate();
		return true;
	}

	return false;
}

void TestState::update(Context const& ctx, float dt) {
	if (elapsed_ += dt; elapsed_ >= 4.f) {
		if (auto director = ctx.director().lock()) {
			director->switch_state(Node::create<TestState2>(ctx));
		}
	}
}

}