#include <Aether.hh>
#ifdef AETHER_DEBUG
	#include <log.hh>
#endif
#include <cstdint>
#include <math_types/vec2.hh>
#include <util/timer.hh>

namespace aether {

Aether::Aether()
    : ctx_(window_, audio_, renderer_, texture_repo_, texture_atlas_repo_, director_)
    , ran_game_loop_(false)
    , is_initialized_(false) {}

Aether::~Aether() {
	if (!is_initialized_ || ran_game_loop_) {
		return;
	}

	if (director_.has_pending_state()) {
		director_.cleanup();
		texture_repo_.clear();
		texture_atlas_repo_.clear();
	}

	audio_.shutdown();
	window_.shutdown();
	is_initialized_ = false;
}

bool Aether::init(std::string_view game_title, size<int> game_resolution, int game_fps) {
	if (is_initialized_) {
		return true;
	}

	if (!window_.init(game_title, game_resolution, game_fps)) {
#ifdef AETHER_DEBUG
		errorlog("Failed");
#endif
		return false;
	}

#ifdef AETHER_DEBUG
	if (!audio_.init()) {
		warninglog("Audio failed to initialize");
	}
#else
	audio_.init();
#endif

	renderer_.reset_render_state();

#ifdef AETHER_DEBUG
	infolog("Initialized");
#endif
	return is_initialized_ = true;
}

void Aether::run() {
	if (!is_initialized_) {
#ifdef AETHER_DEBUG
		errorlog("Can't run loop while uninitialized");
#endif
		return;
	}

	ran_game_loop_ = true;

	while (window_.is_initialized_ && !window_.should_close()) {
		audio_.update();

		bool const is_window_minimized = window_.is_minimized();

		if (!is_window_minimized) {
			ctx_.update_frame_ctx();
			director_.update_current_state(ctx_.delta_time());
		}

		renderer_.start_draw(window_);

		if (!is_window_minimized) {
			director_.draw_current_state();
		}

#ifdef AETHER_DEBUG
		renderer_.end_draw(ctx_);
#else
		renderer_.end_draw();
#endif
	}

	shutdown();
}

Context const& Aether::context() {
	return ctx_;
}

// private
void Aether::shutdown() {
#ifdef AETHER_DEBUG
	infolog("Shutting down");
	auto const start_time = util::timer::start();
#endif

	director_.cleanup();
	texture_repo_.clear();
	texture_atlas_repo_.clear();
	audio_.shutdown();
	window_.shutdown();
	is_initialized_ = false;

#ifdef AETHER_DEBUG
	auto const end_time = util::timer::end(start_time);
	infolog("Done | took {}ms", end_time);
#endif
}

} // namespace ae