#include <aether/Funkin.hh>

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

	}

	shutdown();
}

// private
void Funkin::shutdown() {
	if (window_.is_initialized()) {
		window_.shutdown();
	}
}

}