#include <aether/Funkin.hh>
#include <aether/math/vec2.hh>
#include <aether/common/Callback.hh>
#include <cstdint>
#include <fmt/format.h>

namespace ae {

Funkin::Funkin() :
	is_initialized_(false),
	ctx_(&window_, &renderer_, &resource_, &director_)
{};

Funkin::~Funkin() {
	shutdown();
}

bool Funkin::init(std::string_view game_title, size<int> const& game_resolution, int game_fps) {
	if (is_initialized_) {
		return true;
	}

	if (!window_.init(game_title, game_resolution, game_fps)) {
		// TODO: log error
		return false;
	}

	window_.on_resize(Callback(this, [](void* s) {
		if (Funkin* self = static_cast<Funkin*>(s)) {
			self->update_dpi_scale(self->ctx_);
		}
		})
	);

	is_initialized_ = true;
	return true;
}

void Funkin::run() {
	if (!is_initialized_) {
		return;
	}

	while (!window_.should_close()) {
		bool is_window_minimized = window_.is_minimized();

		if (!is_window_minimized) {
			window_.update();
			director_.update_current_state(ctx_);
			update_frame_ctx(ctx_);
		}
		
		renderer_.start_draw();
		
		if (!is_window_minimized) {
			director_.draw_current_state(ctx_);
#ifdef AETHER_DEBUG
			renderer_.draw_debug(ctx_);
#endif
		}

		renderer_.end_draw();
	}

	shutdown();
}

Context Funkin::context() {
	return Context(&window_, &renderer_, &resource_, &director_);
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