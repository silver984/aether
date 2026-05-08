#include <aether/Funkin.hh>
#include <aether/math/vec2.hh>
#include <aether/common/log.hh>
#include <aether/common/timer.hh>
#include <fmt/format.h>
#include <cstdint>
#include <functional>

namespace ae {

Funkin::Funkin() :
	is_initialized_(false)
{};

Funkin::~Funkin() {
	if (is_initialized_) {
		shutdown();
	}
}

bool Funkin::init(std::string_view game_title, size<int> game_resolution, int game_fps) {
	if (is_initialized_) {
		return true;
	}

	window_ = std::make_shared<Window>();

	if (!window_->init(game_title, game_resolution, game_fps)) {
		window_.reset();
		window_ = nullptr;

		log::error("Failed");
		
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
				renderer_->update_math(ctx_);
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

	auto start_time = timer::start();

	if (director_) {
		director_->try_cleanup();
	}

	if (resource_) {
		resource_->clean_refs();
	}

	if (window_ && window_->is_initialized_) {
		window_->shutdown();
	}

	is_initialized_ = false;

	auto end_time = timer::end(start_time);

	log::info(fmt::format("Done | took {}ms", end_time));
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