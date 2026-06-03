#pragma once
#include <objects/abstract/Scene.hh>

class TestScene final : public aether::Scene {
public:
	TestScene(aether::Context const& ctx);
	~TestScene() override;

protected:
	bool init() override;
	void enter() override;
	void update(float dt) override;
};