#pragma once
#include <abstract/Scene.hh>
#include <memory>

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