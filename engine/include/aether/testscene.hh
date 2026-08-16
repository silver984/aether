#pragma once
#include <aether/scene.hh>

class testscene : public aether::scene {
public:
	testscene() noexcept;
	~testscene() noexcept override;

protected:
	bool init_() override;
	void update_(float dt) override;
};