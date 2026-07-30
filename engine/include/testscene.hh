#pragma once
#include <scene.hh>

class testscene : public aether::scene {
public:
	testscene(aether::context const& ctx) noexcept;
	~testscene() noexcept override;

protected:
	bool init_() override;
	void update_(float dt) override;
};