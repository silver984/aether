#include <aether/Funkin.hh>
#include <aether/internal/rl.hh>
#include <fmt/format.h>

namespace ae {

Funkin::Funkin() :
	is_initialized_(false)
{};

Funkin::~Funkin() = default;

bool Funkin::init(std::string_view game_title, size<int> const& game_resolution, int game_fps) {
	if (window_.init(game_title, game_resolution, game_fps)) {
		is_initialized_ = true;
		return true;
	}

	return false;
}

void Funkin::run() {
	if (!is_initialized_) {
		return;
	}

	while (!window_.should_close()) {
		if (window_.is_minimized()) {
			return;
		}

		update(rl::delta_time());
		draw();
	}

	shutdown();
}

// private
void Funkin::shutdown() {
	if (window_.is_initialized()) {
		window_.shutdown();
	}
}

// private
void Funkin::update(float dt) {
	static int frame_count;
	static float elapsed;
	frame_count++;
	elapsed += dt;
}

// private
void Funkin::draw() const {
	renderer_.start_draw();
	renderer_.end_draw();
}

}