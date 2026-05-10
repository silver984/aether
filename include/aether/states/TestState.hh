#ifndef __AETHER_STATES_TESTSTATE_HH__
#define __AETHER_STATES_TESTSTATE_HH__

#include <aether/objects/abstract/State.hh>
#include <aether/objects/Sprite.hh>
#include <memory>

class TestState : public ae::Node {
public:
	TestState();
	~TestState();

protected:
	bool init() override;
	void update(float dt) override;

private:
	void update_long_trail();

	std::shared_ptr<ae::Sprite> long_;
	std::shared_ptr<ae::Sprite> long_tail_;
	float long_height_;
	float elapsed_;
};

#endif