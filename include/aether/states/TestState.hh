#ifndef __AETHER_STATES_TESTSTATE_HH__
#define __AETHER_STATES_TESTSTATE_HH__

#include <aether/objects/abstract/State.hh>
#include <aether/objects/AnimatedSprite.hh>
#include <aether/objects/Sprite.hh>
#include <aether/objects/TileMap.hh>
#include <memory>

class TestState : public ae::Node {
public:
	TestState(ae::Context const& ctx);
	~TestState();

protected:
	bool init() override;
	void update(float dt) override;

private:
	void update_long_trail();

	std::shared_ptr<ae::AnimatedSprite> animated_;
	std::shared_ptr<ae::Sprite> long_;
	std::shared_ptr<ae::Sprite> long_tail_;
	std::shared_ptr<ae::TileMap> icon_;
	float elapsed_;
};

#endif