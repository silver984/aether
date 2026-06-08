#pragma once
#include <memory>
#include <scene.hh>

class test_scene final : public aether::scene {
public:
	test_scene(aether::context const& ctx);
	~test_scene() override;

protected:
	bool init() override;
	void update(float dt) override;

private:
	float elapsed_;
};