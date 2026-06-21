#pragma once
// #include <nodes/animated_sprite.hh>
#include <scene.hh>
// #include <sound.hh>

class testscene final : public aether::scene {
public:
	testscene(aether::context const& ctx);
	~testscene() override;

protected:
	bool init_() override;
	void update_(float dt) override;

	// private:
	// 	aether::sref<aether::animated_sprite> bf_spin_;
	// 	aether::sref<aether::animated_sprite> og_bf_;
	// 	aether::sref<aether::sound> death_sound_;
	// 	float elapsed_;
	// 	int accumulator_;
};