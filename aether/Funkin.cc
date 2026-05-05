#include <aether/Funkin.hh>
#include <aether/math/vec2.hh>
#include <aether/common/log.hh>
#include <fmt/format.h>
#include <chrono>
#include <cstdint>
#include <functional>
#include <raylib.h>

namespace ae {

Funkin::Funkin() :
	is_initialized_(false)
{};

Funkin::~Funkin() {
	if (is_initialized_) {
		shutdown();
	}
}

bool Funkin::init(std::string_view game_title, size<int> const& game_resolution, int game_fps) {
	if (is_initialized_) {
		return true;
	}

	window_ = std::make_shared<Window>();

	if (!window_->init(game_title, game_resolution, game_fps)) {
		window_.reset();
		window_ = nullptr;
		return false;
	}

	renderer_ = std::make_shared<Renderer>();
	resource_ = std::make_shared<Resource>();
	director_ = std::make_shared<Director>();
	ctx_.store_refs(window_, renderer_, resource_, director_);

	is_initialized_ = true;

	log::info("Initialized");

	return true;
}

void Funkin::run() {
	if (!is_initialized_) {
		return;
	}

	while (window_ && renderer_ && window_->is_initialized_ && !window_->should_close()) {
		bool is_window_minimized = window_->is_minimized();

		if (!is_window_minimized) {
			if (window_->was_resized()) {
				update_dpi_scale();
			}
			
			if (director_) {
				director_->update_current_state(ctx_);
			}

			update_frame_ctx();
		}

		renderer_->start_draw();
		
		if (!is_window_minimized) {
			if (director_) {
				director_->draw_current_state(ctx_);
			}
#ifdef AETHER_DEBUG
			renderer_->draw_debug(ctx_);
#endif
		}

		renderer_->end_draw();
	}

	shutdown();
}

Context const& Funkin::context() {
	return ctx_;
}

// private
void Funkin::shutdown() {
	log::info("Shutting down");

	const auto start_time = std::chrono::high_resolution_clock::now();

	if (window_ && window_->is_initialized_) {
		window_->shutdown();
	}

	is_initialized_ = false;

	const auto end_time = std::chrono::high_resolution_clock::now();
	const auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

	log::info(fmt::format("Took {}ms", time.count()));
}

// private
void Funkin::update_dpi_scale() {
	if (!window_ || !renderer_) {
		return;
	}

	auto screen_size = window_->screen_size_;
	auto render_bounds = renderer_->bounds();
	
	vec2<float> ratio = {
		.x = screen_size.width > 0 ? static_cast<float>(render_bounds.width) / screen_size.width : 0.f,
		.y = screen_size.height > 0 ? static_cast<float>(render_bounds.height) / screen_size.height : 0.f
	};

	ctx_.dpi_scale_ = std::min(ratio.x, ratio.y);
}

// private
void Funkin::update_frame_ctx() {
	static uint32_t frame_count;
	static float elapsed;

	float dt = ctx_.delta_time();
	elapsed += dt;
	ctx_.total_time_ += dt;
	frame_count++;
	
	while (elapsed >= 1.f) {
		ctx_.running_fps_ = frame_count;
		frame_count = 0;
		elapsed -= 1.f;
	}
}

}