#include <chrono>
#include <config.h>
#include <debug/log.hh>
#include <game.hh>
#include <soloud_error.h>
#include <thread>
#include <util/timer.hh>

namespace aether {

game::game() noexcept
        : ctx_(*this)
        , is_initialized_(false) {
}

game::~game() noexcept {
	if (!is_initialized_) {
		return;
	}
	shutdown_();
}

bool game::init(game_init_args const& args) {
	if (is_initialized_) {
		return true;
	}

#if defined(AETHER_ENGINE_HAS_DEBUG)
	if (!log_impl_::create_logfile_()) {
		AETHER_ENGINE_WARNLOG("Couldn't create logfile");
	}
#endif

	if (!aether_resources_.open("aether.res")) {
		AETHER_ENGINE_ERRORLOG("Couldn't open/find resources");
		return false;
	}

	if (!window_.init_(args.window_title, args.resolution, args.fps)) {
		return false;
	}

	using enum SoLoud::SOLOUD_ERRORS;
	SoLoud::result result = soloud_.init();

	if (result != SO_NO_ERROR) {
		AETHER_ENGINE_WARNLOG("SoLoud failed to initialize ? result: {}", result);
	} else {
		AETHER_ENGINE_INFOLOG("SoLoud initialized");
	}

	renderer_.setup2d_();

	AETHER_ENGINE_INFOLOG("Initialized");
	is_initialized_ = true;

	return true;
}

void game::run() {
	if (!is_initialized_) {
		AETHER_ENGINE_ERRORLOG("Can't run loop while uninitialized");
		return;
	}

#if defined(AETHER_ENGINE_HAS_DEBUG)
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
				is_audio_paused = false;
				soloud_.setPauseAll(false);
			}
			window_.update_();
			renderer_.update_viewport_(window_.target_size());
			scene_scheduler_.update_scene_(dt);
		} else {
			if (!is_audio_paused) {
				is_audio_paused = true;
				soloud_.setPauseAll(true);
			}
		}

		renderer_.start_draw_();

		if (!is_window_minimized) {
			scene_scheduler_.draw_scene_();
		}

#if defined(AETHER_ENGINE_HAS_DEBUG)
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

#if defined(AETHER_ENGINE_HAS_DEBUG)
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
	AETHER_ENGINE_INFOLOG("Shutting down");
	util::timer t;
	t.start();

	scene_scheduler_.cleanup_();
	textures_.purge_all_();
	aether_resources_.close();
	soloud_.deinit();
	window_.shutdown_();
	is_initialized_ = false;

	t.stop();
	AETHER_ENGINE_INFOLOG("Done ({}ms)", t.duration());
}

} // namespace aether