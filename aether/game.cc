#include <debug/log.hh>
#include <game.hh>
#include <soloud_error.h>
#include <util/timer.hh>

namespace aether {

game::game()
        : ctx_(window_, renderer_, soloud_, textures_, animations_, audios_, scene_scheduler_)
        , is_initialized_(false) {}

game::~game() {
	if (!is_initialized_) {
		return;
	}

	if (scene_scheduler_.has_pending_scene_()) {
		scene_scheduler_.cleanup_();
		textures_.clear_cache_();
		animations_.clear_cache_();
		audios_.clear_cache_();
	}

	soloud_.deinit();
	window_.shutdown_();
	is_initialized_ = false;
}

bool game::init(init_descriptor const& desc) {
	if (is_initialized_) {
		return true;
	}

	if (!window_.init_(desc.window_title, desc.resolution, desc.fps)) {
		AETHER_ERRORLOG("Failed");
		return false;
	}

#ifdef AETHER_DEBUG
	using enum SoLoud::SOLOUD_ERRORS;
	if (SoLoud::result result = soloud_.init(); result != SO_NO_ERROR) {
		AETHER_WARNLOG("Audio engine failed to initialize | result: {}", result);
	} else {
		AETHER_DEBUGLOG("Audio engine initialized");
	}
#else
	(void)soloud_.init();
#endif

	renderer_.setup2d_();

	using enum sol::lib;
	lua_.open_libraries(base, string, table, math, utf8);

	AETHER_INFOLOG("Initialized");
	return is_initialized_ = true;
}

void game::run() {
	if (!is_initialized_) {
		AETHER_ERRORLOG("Can't run loop while uninitialized");
		return;
	}

	bool is_audio_paused = false;
	auto previous_time   = std::chrono::steady_clock::now();

	while (!window_.should_close_()) {
		auto const current_time        = std::chrono::steady_clock::now();
		float const delta_time         = std::chrono::duration<float>(current_time - previous_time).count();
		previous_time                  = current_time;
		bool const is_window_minimized = window_.is_minimized_();

		if (!is_window_minimized) {
			if (is_audio_paused) {
				is_audio_paused = false;
				soloud_.setPauseAll(is_audio_paused);
			}

			renderer_.update_viewport_(window_.target_size());
			scene_scheduler_.update_scene_(delta_time);
		} else {
			if (!is_audio_paused) {
				is_audio_paused = true;
				soloud_.setPauseAll(is_audio_paused);
			}
		}

		renderer_.start_draw_();

		if (!is_window_minimized) {
			scene_scheduler_.draw_scene_();
		}

		renderer_.end_draw_();
	}

	shutdown_();
}

context const& game::ctx() const {
	return ctx_;
}

void game::shutdown_() {
	AETHER_INFOLOG("Shutting down");
	auto const start_time = util::start();

	scene_scheduler_.cleanup_();
	textures_.clear_cache_();
	animations_.clear_cache_();
	audios_.clear_cache_();
	soloud_.deinit();

	auto const end_time = util::end(start_time);
	AETHER_INFOLOG("Done | took {}ms", end_time);

	window_.shutdown_();
	is_initialized_ = false;
}

} // namespace aether