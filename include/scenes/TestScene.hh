#pragma once
#include <memory>
#include <objects/abstract/Scene.hh>

class TestScene final : public aether::Scene {
public:
	TestScene(aether::Context const& ctx);
	~TestScene() override;

protected:
	bool init() override;
	void update(float dt) override;

private:
	float elapsed_;
};