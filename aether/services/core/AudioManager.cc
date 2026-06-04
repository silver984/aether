#ifdef AETHER_DEBUG
	#include <log.hh>
#endif
#include <algorithm>
#include <cmath>
#include <cstring>
#include <miniaudio/miniaudio.h>
#include <objects/abstract/Sound.hh>
#include <services/core/AudioManager.hh>
#include <util/filesystem.hh>
#include <util/path_map.hh>
#include <util/timer.hh>
#include <utility>

namespace aether {

struct scoped_sound final {
	scoped_sound(std::string_view file) {
		memset(&sound, 0, sizeof(ma_sound));
	}

	~scoped_sound() {
		ma_sound_uninit(&sound);
	}

	ma_sound sound;
};

struct active_sound final {
	active_sound(std::weak_ptr<Sound> _owner)
	    : owner(std::move(_owner)) {}
	std::vector<scoped_sound> instances;
	std::weak_ptr<Sound> owner;
	std::filesystem::path file;
};

struct AudioManager::impl final {
	[[nodiscard]] std::optional<std::uint32_t> generate_key(generation_descriptor const& desc) {
		std::filesystem::path lfile;
		sizeof(std::filesystem::path);

		if (auto const optional_file = util::normalized_filepath(desc.file); optional_file.has_value()) {
			lfile = optional_file.value();
		} else {
#ifdef AETHER_DEBUG
			errorlog("Filesystem gave an error");
#endif
			return std::nullopt;
		}

		if (auto const iterator = sound_lookup.find(lfile); iterator != sound_lookup.end()) {
			return iterator->second;
		}

		active_sounds.emplace(id_hint, desc.owner);
		sound_lookup.emplace(lfile, id_hint);
		return id_hint++;
	}

	bool play(std::uint32_t id) {
		auto const iterator = active_sounds.find(id);

		if (iterator == active_sounds.end()) {
			return false;
		}

		// TODO

		return true;
	}

	bool pause(std::uint32_t id) {
		auto const iterator = active_sounds.find(id);

		if (iterator == active_sounds.end()) {
			return false;
		}

		if (ma_result result = ma_sound_stop(&iterator->second.sound); result != MA_SUCCESS) {
#ifdef AETHER_DEBUG
			errorlog("Failed | error code: {}", (int)result);
#endif
			return false;
		}

		return true;
	}

	[[nodiscard]] float duration(std::uint32_t id) const {
		auto const iterator = active_sounds.find(id);
		if (iterator == active_sounds.end()) {
			return 0.f;
		}
		return (float)iterator->second.pcm_frames / sample_rate;
	}

	[[nodiscard]] float time(std::uint32_t id) const {
		auto const iterator = active_sounds.find(id);
		if (iterator == active_sounds.end()) {
			return 0.f;
		}
		return (float)ma_sound_get_time_in_pcm_frames(&iterator->second.sound) / sample_rate;
	}

	[[nodiscard]] bool is_playing(std::uint32_t id) const {
		auto const iterator = active_sounds.find(id);
		if (iterator == active_sounds.end()) {
			return false;
		}
		return ma_sound_is_playing(&iterator->second.sound);
	}

	[[nodiscard]] bool is_finished(std::uint32_t id) const {
		auto const iterator = active_sounds.find(id);
		if (iterator == active_sounds.end()) {
			return false;
		}
		return ma_sound_at_end(&iterator->second.sound);
	}

	bool seek_time(std::uint32_t id, float seconds) {
		auto const iterator = active_sounds.find(id);

		if (iterator == active_sounds.end()) {
			return false;
		}

		seconds         = std::min(seconds, duration(id));
		ma_uint64 frame = (ma_uint64)std::llround(sample_rate * seconds);

		if (ma_result result = ma_sound_seek_to_pcm_frame(&iterator->second.sound, frame); result != MA_SUCCESS) {
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

		device      = ma_engine_get_device(&engine);
		sample_rate = ma_engine_get_sample_rate(&engine);

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
		std::erase_if(active_sounds, [](auto const& pair) {
			if (ma_sound_is_looping(&pair.second.sound)) {
				return pair.second.owner.expired();
			}

			return pair.second.owner.expired() && ma_sound_at_end(&pair.second.sound);
		});
	}

	void pause_device() {
		device_paused = ma_device_stop(device) == MA_SUCCESS;
	}

	void resume_device() {
		device_paused = !(ma_device_start(device) == MA_SUCCESS);
	}

	[[nodiscard]] bool is_device_paused() const {
		return device_paused;
	}

	util::path_map<std::uint32_t> sound_lookup;                    // ugh... i dont like this at all
	std::unordered_map<std::uint32_t, active_sound> active_sounds; // ugh... i dont like this at all
	ma_device* device = nullptr;
	ma_engine engine;
	std::uint32_t id_hint = 1; // intentionally start with 1
	ma_uint32 sample_rate = 0;
	bool device_paused    = false;
};

// private
AudioManager::AudioManager()
    : impl_(std::make_unique<impl>()) {}

AudioManager::~AudioManager() = default;

std::optional<std::uint32_t> AudioManager::generate_key(generation_descriptor desc) const {
	return impl_->generate_key(desc);
}

bool AudioManager::play(std::uint32_t id) const {
	return impl_->play(id);
}

bool AudioManager::pause(std::uint32_t id) const {
	return impl_->pause(id);
}

float AudioManager::duration(std::uint32_t id) const {
	return impl_->duration(id);
}

float AudioManager::time(std::uint32_t id) const {
	return impl_->time(id);
}

bool AudioManager::is_playing(std::uint32_t id) const {
	return impl_->is_playing(id);
}

bool AudioManager::is_finished(std::uint32_t id) const {
	return impl_->is_finished(id);
}

bool AudioManager::seek_time(std::uint32_t id, float seconds) const {
	return impl_->seek_time(id, seconds);
}

// private
bool AudioManager::init() const {
	return impl_->init();
}

// private
void AudioManager::shutdown() const {
	impl_->shutdown();
}

// private
void AudioManager::update() const {
	impl_->update();
}

// private
void AudioManager::pause_device() const {
	impl_->pause_device();
}

// private
void AudioManager::resume_device() const {
	impl_->resume_device();
}

// private
bool AudioManager::is_device_paused() const {
	return impl_->is_device_paused();
}

} // namespace aether