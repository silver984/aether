#include <aether/game.hh>
#include <aether/log.hh>
#include <aether/timer.hh>
#include <chrono>
#include <soloud_error.h>
#include <thread>

namespace aether {

game::game() noexcept
        : is_initialized_(false) {
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

#if defined(__ae_anydebug__)
	if (!_log_impl::create_logfile()) {
		ae_warn("Couldn't create logfile");
	}
#endif

	if (!window_.init_(args.window_title, args.resolution, args.fps)) {
		return false;
	}

	using enum SoLoud::SOLOUD_ERRORS;
	SoLoud::result result = soloud_.init();

	if (result != SO_NO_ERROR) {
		ae_warn("SoLoud failed to initialize ? result: {}", result);
	} else {
		ae_info("SoLoud initialized");
	}

	renderer_.setup2d_();

	ae_info("Initialized");
	is_initialized_ = true;

	return true;
}

void game::run() {
	if (!is_initialized_) {
		ae_error("Can't run loop while uninitialized");
		return;
	}

#if defined(__ae_anydebug__)
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

#if defined(__ae_anydebug__)
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

#if defined(__ae_anydebug__)
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

void game::shutdown_() {
	ae_info("Shutting down");
	timer t;
	t.start();

	scene_scheduler_.cleanup_();
	textures_.purge_all_();
	soloud_.deinit();
	window_.shutdown_();
	is_initialized_ = false;

	t.stop();
	ae_info("Done ({}ms)", t.duration());
}

} // namespace aether