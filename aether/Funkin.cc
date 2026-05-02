#include <aether/Funkin.hh>
#include <fmt/format.h>
#include <cstdint>

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

	Context ctx = context();

	while (!window_.should_close()) {
		bool is_window_minimized = window_.is_minimized();

		if (!is_window_minimized) {
			update_frame_ctx(ctx);
		}
		
		renderer_.start_draw();
		
		if (!is_window_minimized) {
#ifdef AETHER_DEBUG
			renderer_.draw_debug(ctx);
#endif
		}

		renderer_.end_draw();
	}

	shutdown();
}

Context Funkin::context() {
	return Context(&window_);
}

// private
void Funkin::shutdown() {
	if (window_.is_initialized()) {
		window_.shutdown();
	}
}

// private
void Funkin::update_frame_ctx(Context& ctx) {
	static uint32_t frame_count;
	static float elapsed;

	float dt = ctx.delta_time();
	elapsed += dt;
	ctx.total_time_ += dt;
	frame_count++;
	
	while (elapsed >= 1.f) {
		ctx.running_fps_ = frame_count;
		frame_count = 0;
		elapsed -= 1.f;
	}
}

}