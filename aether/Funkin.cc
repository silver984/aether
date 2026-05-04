#include <aether/Funkin.hh>
#include <aether/math/vec2.hh>
#include <fmt/format.h>
#include <cstdint>

namespace ae {

Funkin::Funkin() :
	is_initialized_(false)
{};

Funkin::~Funkin() {
	shutdown();
}

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
			update_dpi_scale(ctx);
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
	return Context(&window_, &renderer_, &resource_);
}

// private
void Funkin::shutdown() {
	if (!is_initialized_) {
		// already uninitialized
		return;
	}

	if (window_.is_initialized()) {
		window_.shutdown();
	}

	is_initialized_ = false;
}

// private
void Funkin::update_dpi_scale(Context& ctx) {
	auto renderer = ctx.renderer();
	auto window = ctx.window();

	if (!renderer || !window) {
		ctx.dpi_scale_ = 1.f;
		return;
	}

	auto screen_size = window->screen_size();
	auto render_bounds = renderer->bounds();
	
	vec2<float> ratio = {
		.x = screen_size.width > 0 ? static_cast<float>(render_bounds.width) / screen_size.width : 0.f,
		.y = screen_size.height > 0 ? static_cast<float>(render_bounds.height) / screen_size.height : 0.f
	};

	ctx.dpi_scale_ = std::min(ratio.x, ratio.y);
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