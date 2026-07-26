#pragma once
// #include <nodes/animated_sprite.hh>
#include <scene.hh>
// #include <sound.hh>

class testscene : public aether::scene {
public:
	testscene(aether::context const& ctx) noexcept;
	~testscene() noexcept override;

protected:
	bool init_() override;
	// bool init_impl_();
	void update_(float dt) override;
	// void update_impl_(float dt);

	// private:
	// 	aether::ref<aether::animated_sprite> bf_spin_;
	// 	aether::ref<aether::animated_sprite> og_bf_;
	// 	aether::ref<aether::sound> death_sound_;
	// 	float elapsed_;
	// 	int accumulator_;
};