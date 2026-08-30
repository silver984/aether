#pragma once
#include <aether/scene.hh>

class testscene : public aether::scene {
public:
	using aether::scene::scene;

protected:
	bool init_() override;
	void update_(float dt) override;
};