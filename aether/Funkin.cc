#include <aether/Funkin.hh>
#include <aether/math/vec2.hh>
#include <aether/common/log.hh>
#include <aether/common/timer.hh>
#include <fmt/format.h>
#include <cstdint>

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

		errorlog("Failed");
		
		return false;
	}

	renderer_ = std::make_shared<Renderer>();
	resource_ = std::make_shared<Resource>();
	director_ = std::make_shared<Director>();
	ctx_.store_refs(window_, renderer_, resource_, director_);

	is_initialized_ = true;

	infolog("Initialized");

	return true;
}

void Funkin::run() {
	if (!is_initialized_) {
		return;
	}

	while (window_ && renderer_ && window_->is_initialized_ && !window_->should_close()) {
		bool is_window_minimized = window_->is_minimized();

		if (!is_window_minimized) {
			if (director_) {
				director_->update_current_state(ctx_);
			}

			ctx_.update_frame_ctx();
		}

		renderer_->start_draw(ctx_);
		
		if (!is_window_minimized) {
			if (director_) {
				director_->draw_current_state(ctx_);
			}
		}

		renderer_->end_draw(ctx_);
	}

	shutdown();
}

Context const& Funkin::context() {
	return ctx_;
}

// private
void Funkin::shutdown() {
	infolog("Shutting down");

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

	infolog("Done | took {}ms", end_time);
}

}