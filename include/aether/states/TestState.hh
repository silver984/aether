#ifndef __AETHER_STATES_TESTSTATE_HH__
#define __AETHER_STATES_TESTSTATE_HH__

#include <aether/objects/Node.hh>

class TestState : public ae::Node {
public:
	TestState(ae::Context const& ctx);
	~TestState();

protected:
	bool init() override;
};

#endif