#pragma once
#include <aether/objects/abstract/State.hh>

namespace ae {

class TestState : public State {
public:
	TestState();
	~TestState();

protected:
	bool init(Context const& ctx) override;
};

}