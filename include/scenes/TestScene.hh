#pragma once
#include <memory>
#include <objects/abstract/Scene.hh>
#include <objects/abstract/Sound.hh>

class TestScene final : public aether::Scene {
public:
	TestScene(aether::Context const& ctx);
	~TestScene() override;

protected:
	bool init() override;
	void enter() override;

private:
	std::shared_ptr<aether::Sound> sound_;
};