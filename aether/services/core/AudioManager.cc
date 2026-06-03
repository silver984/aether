#ifdef AETHER_DEBUG
	#include <log.hh>
#endif
#include <algorithm>
#include <cstring>
#include <miniaudio/miniaudio.h>
#include <objects/abstract/Sound.hh>
#include <services/core/AudioManager.hh>
#include <unordered_map>
#include <util/filesystem.hh>
#include <util/timer.hh>
#include <utility>

namespace aether {

struct AudioManager::impl {
	struct scoped_sound final {
		scoped_sound(std::weak_ptr<Sound> owner_wref)
		    : owner(std::move(owner_wref)) {
			memset(&sound, 0, sizeof(ma_sound));
		}

		~scoped_sound() {
			ma_sound_uninit(&sound);
		}

		scoped_sound(scoped_sound const&)            = delete;
		scoped_sound(scoped_sound&&)                 = delete;
		scoped_sound& operator=(scoped_sound const&) = delete;
		scoped_sound& operator=(scoped_sound&&)      = delete;

		std::weak_ptr<Sound> owner;
		ma_sound sound;
	};

	[[nodiscard]] std::optional<std::uint32_t> generate_handle(generation_descriptor const& desc) {
		std::filesystem::path lfile;

		if (auto const optional_file = util::normalized_filepath(desc.file); optional_file.has_value()) {
			lfile = optional_file.value();
		} else {
#ifdef AETHER_DEBUG
			errorlog("Filesystem gave an error");
#endif
			return std::nullopt;
		}

#ifdef AETHER_VERBOSE_DEBUG
		debuglog("Generating audio handle | file: \"{}\"", lfile.filename().string());
		auto const start_time = util::start();
#endif

		auto [iterator, _] = active_sounds.emplace(id_hint, desc.owner);

		if (ma_result result =
		        ma_sound_init_from_file(&engine, lfile.string().c_str(), 0, nullptr, nullptr, &iterator->second.sound);
		    result != MA_SUCCESS) {
#ifdef AETHER_DEBUG
			errorlog("Failed to initialize sound | error code: {}", (int)result);
#endif
			active_sounds.erase(iterator);
			return std::nullopt;
		}

#ifdef AETHER_VERBOSE_DEBUG
		auto const end_time = util::end(start_time);
		ma_uint64 pcm_frames;
		ma_sound_get_length_in_pcm_frames(&iterator->second.sound, &pcm_frames);
		tracelog("Successful generation | handle owner: {} | pcm frames: {}", fmt::ptr(desc.owner.get()), pcm_frames);
		debuglog("Done | took {}ms", end_time);
#endif

		return id_hint++;
	}

	bool play(std::uint32_t id) {
		auto iterator = active_sounds.find(id);

		if (iterator == active_sounds.end()) {
			return false;
		}

		if (ma_result result = ma_sound_start(&iterator->second.sound); result != MA_SUCCESS) {
#ifdef AETHER_DEBUG
			errorlog("Failed | error code: {}", (int)result);
#endif
			return false;
		}

		return true;
	}

	bool init() {
		if (ma_result result = ma_engine_init(nullptr, &engine); result != MA_SUCCESS) {
#ifdef AETHER_DEBUG
			errorlog("Couldn't initialize engine | error code: {}", (int)result);
#endif
			return false;
		}

		device = ma_engine_get_device(&engine);

#ifdef AETHER_VERBOSE_DEBUG
		tracelog("Device address: {}", fmt::ptr(device));
		debuglog("Initialized");
#endif
		return true;
	}

	void shutdown() {
		active_sounds.clear();
		ma_engine_uninit(&engine);
	}

	void update() {
		std::erase_if(active_sounds, [](auto& pair) {
			if (ma_sound_is_looping(&pair.second.sound)) {
				return pair.second.owner.expired();
			}

			return pair.second.owner.expired() && ma_sound_at_end(&pair.second.sound);
		});
	}

	void pause() {
		is_audio_paused = ma_device_stop(device) == MA_SUCCESS;
	}

	void resume() {
		is_audio_paused = !(ma_device_start(device) == MA_SUCCESS);
	}

	[[nodiscard]] bool is_paused() {
		return is_audio_paused;
	}

	std::unordered_map<std::uint32_t, scoped_sound> active_sounds;
	ma_device* device = nullptr;
	ma_engine engine;
	std::uint32_t id_hint = 0;
	bool is_audio_paused  = false;
};

// private
AudioManager::AudioManager()
    : impl_(std::make_unique<impl>()) {}

AudioManager::~AudioManager() = default;

std::optional<std::uint32_t> AudioManager::generate_handle(generation_descriptor desc) {
	return impl_->generate_handle(desc);
}

bool AudioManager::play(std::uint32_t id) {
	return impl_->play(id);
}

// private
bool AudioManager::init() {
	return impl_->init();
}

// private
void AudioManager::shutdown() {
	impl_->shutdown();
}

// private
void AudioManager::update() {
	impl_->update();
}

// private
void AudioManager::pause() {
	impl_->pause();
}

// private
void AudioManager::resume() {
	impl_->resume();
}

// private
bool AudioManager::is_paused() {
	return impl_->is_paused();
}

} // namespace aether