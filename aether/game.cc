#include <context.hh>
#include <game.hh>
#ifdef AETHER_DEBUG
	#include <debug/log.hh>
#endif
#include <animation_repository.hh>
#include <audio_repository.hh>
#include <chrono>
#include <renderer.hh>
#include <scene_scheduler.hh>
#include <sol/state.hpp>
#include <soloud.h>
#include <soloud_error.h>
#include <texture_repository.hh>
#include <util/timer.hh>
#include <window.hh>

using enum SoLoud::SOLOUD_ERRORS;

namespace aether {

struct game::impl final {
	impl()
	        : ctx_(window_, renderer_, soloud_, textures_, animations_, audios_, scene_scheduler_)
	        , is_initialized_(false) {}

	~impl() {
		if (!is_initialized_) {
			return;
		}

		if (scene_scheduler_.has_pending_scene()) {
			scene_scheduler_.cleanup();
			textures_.clear_cache();
			animations_.clear_cache();
		}

		soloud_.deinit();
		window_.shutdown();
		is_initialized_ = false;
	}

	bool init(init_descriptor const& desc) {
		if (is_initialized_) {
			return true;
		}

		if (!window_.init(desc.window_title, desc.resolution, desc.fps)) {
#ifdef AETHER_DEBUG
			errorlog("Failed");
#endif
			return false;
		}

#ifdef AETHER_DEBUG
		if (SoLoud::result result = soloud_.init(); result != SO_NO_ERROR) {
			warninglog("Audio engine failed to initialize | result: {}", result);
		} else {
	#ifdef AETHER_VERBOSE_DEBUG
			debuglog("Audio engine initialized");
	#endif
		}
#else
		(void)soloud_.init();
#endif

		renderer_.setup(window_);

		using enum sol::lib;
		lua_.open_libraries(base, string, table, math, utf8);

#ifdef AETHER_DEBUG
		infolog("Initialized");
#endif
		return is_initialized_ = true;
	}

	void run() {
		if (!is_initialized_) {
#ifdef AETHER_DEBUG
			errorlog("Can't run loop while uninitialized");
#endif
			return;
		}

		auto previous_time = std::chrono::steady_clock::now();

		while (!window_.should_close()) {
			auto const current_time        = std::chrono::steady_clock::now();
			float const delta_time         = std::chrono::duration<float>(current_time - previous_time).count();
			previous_time                  = current_time;
			bool const is_window_minimized = window_.is_minimized();

			if (!is_window_minimized) {
				window_.update();
				// ctx_.update_frame_context();
				scene_scheduler_.update_scene(delta_time);
			}

			renderer_.start_draw();

			if (!is_window_minimized) {
				scene_scheduler_.draw_scene();
			}

			// #ifdef AETHER_DEBUG
			// 			renderer_.end_draw(ctx_.running_fps());
			// #else
			// 			renderer_.end_draw();
			// #endif

			renderer_.end_draw();
		}

		shutdown();
	}

	void shutdown() {
#ifdef AETHER_DEBUG
		infolog("Shutting down");
		auto const start_time = util::start();
#endif

		scene_scheduler_.cleanup();
		textures_.clear_cache();
		animations_.clear_cache();
		soloud_.deinit();
		window_.shutdown();
		is_initialized_ = false;

#ifdef AETHER_DEBUG
		auto const end_time = util::end(start_time);
		infolog("Done | took {}ms", end_time);
#endif
	}

	window window_;
	renderer renderer_;
	SoLoud::Soloud soloud_;
	sol::state lua_;
	scene_scheduler scene_scheduler_;
	texture_repository textures_;
	animation_repository animations_;
	audio_repository audios_;
	context ctx_;
	bool is_initialized_;
};

game::game()
        : impl_(std::make_unique<impl>()) {}

game::~game() = default;

bool game::init(init_descriptor const& desc) const {
	return impl_->init(desc);
}

void game::run() const {
	impl_->run();
}

context const& game::fetch_context() const {
	return impl_->ctx_;
}

} // namespace aether