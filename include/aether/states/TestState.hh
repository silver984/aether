#pragma once
#include <aether/objects/Node.hh>

class TestState : public ae::Node {
public:
	TestState(ae::Context const& ctx);
	~TestState();

protected:
	bool init() override;
};