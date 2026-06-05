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
#include <soloud.h>
#include <soloud_error.h>
#include <util/timer.hh>

using enum SoLoud::SOLOUD_ERRORS;

namespace aether {

struct Aether::impl final {
	impl()
	    : ctx(window, renderer, soloud, texture_repository, animation_repository, audio_repository, scene_scheduler)
	    , is_initialized(false) {}

	~impl() {
		if (!is_initialized) {
			return;
		}

		if (scene_scheduler.has_pending_scene()) {
			scene_scheduler.cleanup();
			texture_repository.clear_cache();
			animation_repository.clear_cache();
		}

		soloud.deinit();
		window.shutdown();
		is_initialized = false;
	}

	bool init(init_descriptor const& desc) {
		if (is_initialized) {
			return true;
		}

		if (!window.init({.title = desc.window_title, .resolution = desc.resolution, .fps = desc.fps})) {
#ifdef AETHER_DEBUG
			errorlog("Failed");
#endif
			return false;
		}

#ifdef AETHER_DEBUG
		if (SoLoud::result result = soloud.init(); result != SO_NO_ERROR) {
			warninglog("Audio engine failed to initialize | result: {}", result);
		} else {
	#ifdef AETHER_VERBOSE_DEBUG
			debuglog("Audio engine initialized");
	#endif
		}
#else
		(void)soloud.init();
#endif

		renderer.setup(window);

#ifdef AETHER_DEBUG
		infolog("Initialized");
#endif
		return is_initialized = true;
	}

	void run() {
		if (!is_initialized) {
#ifdef AETHER_DEBUG
			errorlog("Can't run loop while uninitialized");
#endif
			return;
		}

		while (!window.should_close()) {
			window.update();

			bool const is_window_minimized = window.is_minimized();

			if (!is_window_minimized) {
				ctx.update_frame_context();
				scene_scheduler.update_scene(ctx.delta_time());
			}

			renderer.start_draw();

			if (!is_window_minimized) {
				scene_scheduler.draw_scene();
			}

#ifdef AETHER_DEBUG
			renderer.end_draw(ctx.running_fps());
#else
			renderer.end_draw();
#endif
		}

		shutdown();
	}

	void shutdown() {
#ifdef AETHER_DEBUG
		infolog("Shutting down");
		auto const start_time = util::start();
#endif

		scene_scheduler.cleanup();
		texture_repository.clear_cache();
		animation_repository.clear_cache();
		soloud.deinit();
		window.shutdown();
		is_initialized = false;

#ifdef AETHER_DEBUG
		auto const end_time = util::end(start_time);
		infolog("Done | took {}ms", end_time);
#endif
	}

	Window window;
	Renderer renderer;
	SoLoud::Soloud soloud;
	SceneScheduler scene_scheduler;
	TextureRepository texture_repository;
	AnimationRepository animation_repository;
	AudioRepository audio_repository;
	Context ctx;
	bool is_initialized;
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
	return impl_->ctx;
}

} // namespace aether