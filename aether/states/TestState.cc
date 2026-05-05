#include <aether/states/TestState.hh>
#include <aether/objects/Graphic.hh>

namespace ae {

TestState::TestState() = default;
TestState::~TestState() = default;

bool TestState::init(Context const& ctx) {
	if (auto grah = Node::create<Graphic>(ctx, "resources/grah.png")) {
		this->add(grah);
		return true;
	}

	return false;
}

}