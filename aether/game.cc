#include <debug/heap.hh>
#include <debug/log.hh>
#include <debug/win32.hh>
#include <game.hh>
#include <soloud_error.h>
#include <thread>
#include <util/timer.hh>

namespace aether {

game::game()
        : ctx_(context::descriptor{.rwindow          = window_,
                                   .rrenderer        = renderer_,
                                   .rtextures        = textures_,
                                   .ranimations      = animations_,
                                   .raudios          = audios_,
                                   .rscene_scheduler = scene_scheduler_,
                                   .rsoloud          = soloud_})
        , is_initialized_(false) {}

game::~game() {
	if (!is_initialized_) {
		return;
	}

	AETHER_INFOLOG("Shutting down");
	auto const start_time = util::start();

	if (scene_scheduler_.has_pending_scene_()) {
		scene_scheduler_.cleanup_();
		textures_.clear_cache_();
		animations_.clear_cache_();
		audios_.clear_cache_();
	}

	soloud_.deinit();
	window_.shutdown_();
	is_initialized_ = false;

	auto const end_time = util::end(start_time);
	AETHER_INFOLOG("Done | took {}ms", end_time);
#if defined(AETHER_DEBUG) || defined(AETHER_RELWITHDEB)
	log::impl_::close_log_file_();
#endif
}

bool game::init(init_descriptor const& desc) {
	if (is_initialized_) {
		return true;
	}

#if defined(AETHER_DEBUG) || defined(AETHER_RELWITHDEB)
	log::impl_::create_log_file_();

	#if !defined(AETHER_RELWITHDEB) && defined(WIN32)
	if (!win32_::enable_console_colors_()) {
		AETHER_WARNLOG("Couldn't enable console colors");
	}
	#endif
#endif

	if (!window_.init_(desc.window_title, desc.resolution, desc.fps)) {
		AETHER_ERRORLOG("Window failed to initialized");
		return false;
	} else {
		AETHER_DEBUGLOG("Window initialized");
	}

#if defined(AETHER_DEBUG) || defined(AETHER_RELWITHDEB)
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

#if defined(AETHER_DEBUG) || defined(AETHER_RELWITHDEB)
	uint32_t frame_count   = 0;
	uint32_t evaluated_fps = 0;
	float accumulator      = 0.f;
#endif

	using namespace std::chrono;
	bool is_audio_paused         = false;
	auto previous_time           = steady_clock::now();
	auto next_frame_time         = previous_time;
	auto const target_frame_time = duration_cast<steady_clock::duration>(duration<float>(1.f / window_.target_fps()));

	while (!window_.should_close_()) {
		auto const start_time          = steady_clock::now();
		float const dt                 = duration<float>(start_time - previous_time).count();
		previous_time                  = start_time;
		bool const is_window_minimized = window_.is_minimized_();

		if (!is_window_minimized) {
			if (is_audio_paused) {
				is_audio_paused = false;
				soloud_.setPauseAll(is_audio_paused);
			}

			window_.update_();
			renderer_.update_viewport_(window_.target_size());
			scene_scheduler_.update_scene_(dt);
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

#if defined(AETHER_DEBUG) || defined(AETHER_RELWITHDEB)
		double divider = 1024.0 * 1024.0;
		renderer_.end_draw_(evaluated_fps, heap::usage() / divider, heap::total_usage() / divider);
#else
		renderer_.end_draw_();
#endif

		if (is_window_minimized) {
			next_frame_time = steady_clock::now();
			std::this_thread::sleep_for(milliseconds(100));
			continue;
		}

		next_frame_time += target_frame_time;
		std::this_thread::sleep_until(next_frame_time);

#if defined(AETHER_DEBUG) || defined(AETHER_RELWITHDEB)
		++frame_count;
		accumulator += dt;
		while (accumulator >= 1.f) {
			evaluated_fps = frame_count;
			frame_count   = 0;
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
	auto const start_time = util::start();

	scene_scheduler_.cleanup_();
	textures_.clear_cache_();
	animations_.clear_cache_();
	audios_.clear_cache_();
	soloud_.deinit();
	window_.shutdown_();
	is_initialized_ = false;

	auto const end_time = util::end(start_time);
	AETHER_INFOLOG("Done | took {}ms", end_time);
#if defined(AETHER_DEBUG) || defined(AETHER_RELWITHDEB)
	log::impl_::close_log_file_();
#endif
}

} // namespace aether