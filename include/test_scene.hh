#pragma once
// #include <nodes/animated_sprite.hh>
#include <scene.hh>
// #include <sound.hh>

class test_scene final : public aether::scene {
public:
	test_scene(aether::context const& ctx);
	~test_scene() override;

protected:
	bool init_() override;
	// void update_(float dt) override;

	// private:
	// 	aether::sref<aether::animated_sprite> bf_spin_;
	// 	aether::sref<aether::animated_sprite> og_bf_;
	// 	aether::sref<aether::sound> death_sound_;
	// 	float elapsed_;
	// 	int accumulator_;
};