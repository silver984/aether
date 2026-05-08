#ifndef __AETHER_STATES_TESTSTATE2_HH__
#define __AETHER_STATES_TESTSTATE2_HH__

#include <aether/objects/abstract/State.hh>

namespace ae {

class TestState2 : public State {
public:
	TestState2();
	~TestState2();

protected:
	bool init(Context const& ctx) override;
	void update(Context const& ctx, float dt) override;

private:
	float elapsed_;
};

}

#endif