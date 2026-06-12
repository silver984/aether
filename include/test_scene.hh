#pragma once
#include <scene.hh>
#include <sound.hh>
#include <nodes/animated_sprite.hh>

class test_scene final : public aether::scene {
public:
	test_scene(aether::context const& ctx);
	~test_scene() override;

protected:
	bool init_() override;
	void update_(float dt) override;

private:
	aether::sref<aether::sound> sound_;
	float elapsed_;
	
	aether::sref<aether::animated_sprite> bf_spin_;
};