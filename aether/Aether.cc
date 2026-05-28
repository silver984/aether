#include <aether/Aether.hh>
#include <aether/debug/log.hh>
#include <aether/math_types/vec2.hh>
#include <aether/util/timer.hh>
#include <cstdint>

namespace ae {

Aether::Aether() : ctx_(window_, renderer_, texture_repo_, texture_atlas_repo_, director_), is_initialized_(false) {}

Aether::~Aether() {
	if (is_initialized_) {
		shutdown();
	}
}

bool Aether::init(std::string_view game_title, size<int> game_resolution, int game_fps) {
	if (is_initialized_) {
		return true;
	}

	if (!window_.init(game_title, game_resolution, game_fps)) {
		errorlog("Failed");
		return false;
	}

	renderer_.bind_context(ctx_);
	director_.bind_context(ctx_);

	infolog("Initialized");
	return is_initialized_ = true;
}

void Aether::run() {
	if (!is_initialized_) {
		debuglog("Attempted to run loop while uninitialized");
		return;
	}

	while (window_.is_initialized_ && !window_.should_close()) {
		bool const is_window_minimized = window_.is_minimized();

		if (!is_window_minimized) {
			ctx_.update_frame_ctx();
			director_.update_current_state();
		}

		renderer_.start_draw();

		if (!is_window_minimized) {
			director_.draw_current_state();
		}

		renderer_.end_draw();
	}

	shutdown();
}

Context const& Aether::context() {
	return ctx_;
}

// private
void Aether::shutdown() {
	infolog("Shutting down");
	auto const start_time = util::timer::start();

	director_.try_cleanup();
	texture_repo_.clear();
	texture_atlas_repo_.clear();

	if (window_.is_initialized_) {
		window_.shutdown();
	}

	is_initialized_ = false;

	auto const end_time = util::timer::end(start_time);
	infolog("Done | took {}ms", end_time);
}

} // namespace ae