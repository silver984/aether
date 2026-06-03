#include <Aether.hh>
#ifdef AETHER_DEBUG
	#include <log.hh>
#endif
#include <cstdint>
#include <math/vec2.hh>
#include <util/timer.hh>

namespace aether {

Aether::Aether()
    : ctx_(window_, audio_manager_, renderer_, texture_repository_, animation_repository_, scene_scheduler_)
    , ran_game_loop_(false)
    , is_initialized_(false) {}

Aether::~Aether() {
	if (!is_initialized_ || ran_game_loop_) {
		return;
	}

	if (scene_scheduler_.has_pending_scene()) {
		scene_scheduler_.cleanup();
		texture_repository_.clear();
		animation_repository_.clear();
	}

	audio_manager_.shutdown();
	window_.shutdown();
	is_initialized_ = false;
}

bool Aether::init(init_descriptor desc) {
	if (is_initialized_) {
		return true;
	}

	if (!window_.init({.title = desc.window_title, .resolution = desc.resolution, .fps = desc.fps})) {
#ifdef AETHER_DEBUG
		errorlog("Failed");
#endif
		return false;
	}

#ifdef AETHER_DEBUG
	if (!audio_manager_.init()) {
		warninglog("AudioManager failed to initialize");
	}
#else
	(void)audio_manager_.init();
#endif

	renderer_.setup(window_);

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

	while (!window_.should_close()) {
		window_.update();

		bool const is_window_minimized = window_.is_minimized();

		if (!is_window_minimized) {
			if (audio_manager_.is_paused()) {
				audio_manager_.resume();
			}

			audio_manager_.update();
			ctx_.update_frame_context();
			scene_scheduler_.update_scene(ctx_.delta_time());
		} else {
			if (!audio_manager_.is_paused()) {
				audio_manager_.pause();
			}
		}

		renderer_.start_draw();

		if (!is_window_minimized) {
			scene_scheduler_.draw_scene();
		}

#ifdef AETHER_DEBUG
		renderer_.end_draw(ctx_.running_fps());
#else
		renderer_.end_draw();
#endif
	}

	ran_game_loop_ = true;
	shutdown();
}

Context const& Aether::context() {
	return ctx_;
}

// private
void Aether::shutdown() {
#ifdef AETHER_DEBUG
	infolog("Shutting down");
	auto const start_time = util::start();
#endif

	scene_scheduler_.cleanup();
	texture_repository_.clear();
	animation_repository_.clear();
	audio_manager_.shutdown();
	window_.shutdown();
	is_initialized_ = false;

#ifdef AETHER_DEBUG
	auto const end_time = util::end(start_time);
	infolog("Done | took {}ms", end_time);
#endif
}

} // namespace aether