#include <aether/Aether.hh>
#include <aether/debug/log.hh>
#include <aether/math_types/vec2.hh>
#include <aether/util/timer.hh>
#include <cstdint>

namespace ae {

Aether::Aether() : is_initialized_(false) {}
Aether::~Aether() {
	if (is_initialized_) {
		shutdown();
	}
}

bool Aether::init(std::string_view game_title, size<int> game_resolution, int game_fps) {
	if (is_initialized_) {
		return true;
	}

	window_ = std::make_shared<Window>();

	if (!window_->init(game_title, game_resolution, game_fps)) {
		window_.reset();
		errorlog("Failed");
		return false;
	}

	renderer_ = std::make_shared<Renderer>(ctx_);
	resource_ = std::make_shared<Resource>();
	director_ = std::make_shared<Director>(ctx_);
	ctx_.store_refs(window_, renderer_, resource_, director_);

	infolog("Initialized");
	return is_initialized_ = true;
}

void Aether::run() {
	if (!is_initialized_) {
		debuglog("Attempted to run loop uninitialized");
		return;
	}

	while (window_ && renderer_ && window_->is_initialized_ && !window_->should_close()) {
		bool is_window_minimized = window_->is_minimized();

		if (!is_window_minimized) {
			ctx_.update_frame_ctx();

			if (director_) {
				director_->update_current_state();
			}
		}

		renderer_->start_draw();

		if (!is_window_minimized) {
			if (director_) {
				director_->draw_current_state();
			}
		}

		renderer_->end_draw();
	}

	shutdown();
}

Context const& Aether::context() {
	return ctx_;
}

// private
void Aether::shutdown() {
	infolog("Shutting down");

	auto start_time = timer::start();

	if (director_) {
		director_->try_cleanup();
	}

	if (resource_) {
		resource_->try_clean_refs();
	}

	if (window_ && window_->is_initialized_) {
		window_->shutdown();
	}

	is_initialized_ = false;

	auto end_time = timer::end(start_time);
	infolog("Done | took {}ms", end_time);
}

} // namespace ae