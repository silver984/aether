#include <aether/game.hh>
#include <aether/log.hh>
#include <aether/renderer.hh>
#include <aether/timer.hh>
#include <aether/window.hh>

#include <soloud_error.h>

#include <chrono>
#include <stdexcept>
#include <thread>

namespace aether {

game::~game() {
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
	if (!_log_impl::create_logfile_()) {
		ae_warn("Couldn't create logfile");
	}
#endif

	try {
		_window_impl::try_init_(args.window_title, args.resolution, args.fps);
	} catch (std::runtime_error const&) {
		throw;
	}

	using enum SoLoud::SOLOUD_ERRORS;
	SoLoud::result result = soloud_.init();

	if (result != SO_NO_ERROR) {
		ae_warn("SoLoud failed to initialize ? result: {}", result);
	} else {
		ae_info("SoLoud initialized");
	}

	_renderer_impl::setup_2d_();

	ae_info("Initialized");
	is_initialized_ = true;

	return true;
}

void game::run(unique_ref<scene> s) {
	if (!is_initialized_) {
		ae_error("Can't run loop while uninitialized");
		return;
	}

	scene_scheduler_.replace_scene(std::move(s));

	bool is_audio_paused = false;
	auto last_frametime  = std::chrono::steady_clock::now();
	auto next_frametime  = last_frametime;

	while (!_window_impl::should_close_()) {
		auto const now                 = std::chrono::steady_clock::now();
		float const dt                 = std::chrono::duration<float>(now - last_frametime).count();
		last_frametime                 = now;
		bool const is_window_minimized = _window_impl::is_minimized_();

		if (!is_window_minimized) {
			if (is_audio_paused) {
				soloud_.setPauseAll(is_audio_paused = false);
			}
			scene_scheduler_.update_scene_(dt);
		} else {
			if (!is_audio_paused) {
				soloud_.setPauseAll(is_audio_paused = true);
			}
		}

		_renderer_impl::start_draw_();

		if (!is_window_minimized) {
			scene_scheduler_.draw_scene_();
		}

		_renderer_impl::end_draw_();

		if (is_window_minimized) {
			next_frametime = std::chrono::steady_clock::now();
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			continue;
		}

		std::chrono::duration<float> raw_dt(1.f / window::fps());
		next_frametime += std::chrono::duration_cast<std::chrono::steady_clock::duration>(raw_dt);
		std::this_thread::sleep_until(next_frametime);
	}

	shutdown_();
}

context game::ctx() {
	return {
	        .scene_scheduler = &scene_scheduler_,
	        .textures        = &textures_,
	};
}

void game::shutdown_() {
	ae_info("Shutting down");
	timer t;
	t.start();

	scene_scheduler_.cleanup_();
	textures_.purge_all_();
	soloud_.deinit();
	_window_impl::close_();
	is_initialized_ = false;

	t.stop();
	ae_info("Done ({}ms)", t.duration());
}

} // namespace aether