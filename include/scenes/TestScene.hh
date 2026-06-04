#pragma once
#include <memory>
#include <objects/abstract/Scene.hh>

namespace aether {

class Sound;

}

class TestScene final : public aether::Scene {
public:
	TestScene(aether::Context const& ctx);
	~TestScene() override;

protected:
	bool init() override;
	void enter() override;
	void update(float dt) override;

private:
	std::shared_ptr<aether::Sound> sound_;
	float elapsed_;
};