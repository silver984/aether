#include <chrono>
#include <debug/log.hh>
#include <game.hh>
#include <soloud_error.h>
#include <thread>
#include <util/timer.hh>

namespace aether {

game::game()
        : ctx_(*this)
        , is_initialized_(false) {
}

game::~game() {
	if (!is_initialized_) {
		return;
	}

	AETHER_INFOLOG("Shutting down");
	util::timer t;
	t.start();

	if (scene_scheduler_.has_pending_scene_()) {
		scene_scheduler_.cleanup_();
		textures_.purge_all_();
		// animations_.clear_cache_();
		// audios_.clear_cache_();
	}

	// lua_manager_.shutdown_();
	aether_resources_.close();
	soloud_.deinit();
	window_.shutdown_();
	is_initialized_ = false;

	t.stop();
	AETHER_INFOLOG("Done ({}ms)", t.duration());
#if defined(AETHER_DEBUG) || defined(AETHER_RELWITHDEB)
	log_impl_::close_logfile_();
#endif
}

bool game::init(init_descriptor const& desc) {
	if (is_initialized_) {
		return true;
	}

#if defined(AETHER_DEBUG) || defined(AETHER_RELWITHDEB)
	if (!log_impl_::create_logfile_()) {
		AETHER_WARNLOG("Couldn't create logfile");
	}
#endif

	if (!window_.init_(desc.window_title, desc.resolution, desc.fps)) {
		return false;
	}

	if (!aether_resources_.open("aether.res")) {
		AETHER_ERRORLOG("Couldn't open/find resources");
		return false;
	}

	using enum SoLoud::SOLOUD_ERRORS;
	if (SoLoud::result result = soloud_.init(); result != SO_NO_ERROR) {
		AETHER_WARNLOG("SoLoud failed to initialize ? result: {}", result);
	} else {
		AETHER_INFOLOG("SoLoud initialized");
	}

	renderer_.setup2d_();
	// lua_manager_.init_();
	// lua_manager_.run_scripts_();

	AETHER_INFOLOG("Initialized");
	return is_initialized_ = true;
}

void game::run() {
	if (!is_initialized_) {
		AETHER_ERRORLOG("Can't run loop while uninitialized");
		return;
	}

#if defined(AETHER_DEBUG) || defined(AETHER_RELWITHDEB)
	uint32_t framecount = 0;
	uint32_t evalfps    = 0;
	float accumulator   = 0.f;
#endif

	using namespace std::chrono;
	using clock = steady_clock;

	bool is_audio_paused = false;
	auto last_frametime  = clock::now();
	auto next_frametime  = last_frametime;

	while (!window_.should_close_()) {
		auto const now                 = clock::now();
		float const dt                 = duration<float>(now - last_frametime).count();
		last_frametime                 = now;
		bool const is_window_minimized = window_.is_minimized_();

		if (!is_window_minimized) {
			if (is_audio_paused) {
				soloud_.setPauseAll(is_audio_paused = false);
			}
			window_.update_();
			renderer_.update_viewport_(window_.target_size());
			scene_scheduler_.update_scene_(dt);
		} else {
			if (!is_audio_paused) {
				soloud_.setPauseAll(is_audio_paused = true);
			}
		}

		renderer_.start_draw_();

		if (!is_window_minimized) {
			scene_scheduler_.draw_scene_();
		}

#if defined(AETHER_DEBUG) || defined(AETHER_RELWITHDEB)
		renderer_.end_draw_(evalfps, dt);
#else
		renderer_.end_draw_();
#endif
		if (is_window_minimized) {
			next_frametime = clock::now();
			std::this_thread::sleep_for(milliseconds(100));
			continue;
		}

		next_frametime += duration_cast<clock::duration>(duration<float>(1.f / window_.target_fps()));
		std::this_thread::sleep_until(next_frametime);

#if defined(AETHER_DEBUG) || defined(AETHER_RELWITHDEB)
		++framecount;
		accumulator += dt;
		while (accumulator >= 1.f) {
			evalfps    = framecount;
			framecount = 0;
			accumulator -= 1.f;
		}
#endif
	}

	shutdown_();
}

context const& game::ctx() const {
	return ctx_;
}

void game::shutdown_() {
	AETHER_INFOLOG("Shutting down");
	util::timer t;
	t.start();

	scene_scheduler_.cleanup_();
	textures_.purge_all_();
	// animations_.clear_cache_();
	// audios_.clear_cache_();
	// lua_manager_.shutdown_();
	aether_resources_.close();
	soloud_.deinit();
	window_.shutdown_();
	is_initialized_ = false;

	t.stop();
	AETHER_INFOLOG("Done ({}ms)", t.duration());
#if defined(AETHER_DEBUG) || defined(AETHER_RELWITHDEB)
	log_impl_::close_logfile_();
#endif
}

} // namespace aether