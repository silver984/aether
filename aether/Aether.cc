#include <Aether.hh>
#include <Context.hh>
#ifdef AETHER_DEBUG
	#include <log.hh>
#endif
#include <services/SceneScheduler.hh>
#include <services/core/Renderer.hh>
#include <services/core/Window.hh>
#include <services/resource/AnimationRepository.hh>
#include <services/resource/AudioRepository.hh>
#include <services/resource/TextureRepository.hh>
#include <sol/state.hpp>
#include <soloud.h>
#include <soloud_error.h>
#include <util/timer.hh>

using enum SoLoud::SOLOUD_ERRORS;

namespace aether {

struct Aether::impl final {
	impl()
	    : ctx_(window_, renderer_, soloud_, textures, animations_, audios_, scene_scheduler_)
	    , is_initialized_(false) {}

	~impl() {
		if (!is_initialized_) {
			return;
		}

		if (scene_scheduler_.has_pending_scene()) {
			scene_scheduler_.cleanup();
			textures.clear_cache();
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

		if (!window_.init({.title = desc.window_title, .resolution = desc.resolution, .fps = desc.fps})) {
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

		while (!window_.should_close()) {
			window_.update();

			bool const is_window_minimized = window_.is_minimized();

			if (!is_window_minimized) {
				ctx_.update_frame_context();
				scene_scheduler_.update_scene(ctx_.delta_time());
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

		shutdown();
	}

	void shutdown() {
#ifdef AETHER_DEBUG
		infolog("Shutting down");
		auto const start_time = util::start();
#endif

		scene_scheduler_.cleanup();
		textures.clear_cache();
		animations_.clear_cache();
		soloud_.deinit();
		window_.shutdown();
		is_initialized_ = false;

#ifdef AETHER_DEBUG
		auto const end_time = util::end(start_time);
		infolog("Done | took {}ms", end_time);
#endif
	}

	Window window_;
	Renderer renderer_;
	SoLoud::Soloud soloud_;
	sol::state lua_;
	SceneScheduler scene_scheduler_;
	TextureRepository textures;
	AnimationRepository animations_;
	AudioRepository audios_;
	Context ctx_;
	bool is_initialized_;
};

Aether::Aether()
    : impl_(std::make_unique<impl>()) {}

Aether::~Aether() = default;

bool Aether::init(init_descriptor desc) const {
	return impl_->init(desc);
}

void Aether::run() const {
	impl_->run();
}

Context const& Aether::context() const {
	return impl_->ctx_;
}

} // namespace aether