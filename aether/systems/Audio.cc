#ifdef AETHER_DEBUG
	#include <aether/debug/log.hh>
#endif
#include <aether/objects/abstract/Sound.hh>
#include <aether/systems/Audio.hh>
#include <aether/util/filesystem.hh>
#include <aether/util/timer.hh>
#include <algorithm>
#include <miniaudio/miniaudio.h>
#include <unordered_map>
#include <utility>

namespace ae {

struct Audio::impl {
	struct scoped_sound final {
		explicit scoped_sound(std::weak_ptr<Sound> owner_wref) : owner(std::move(owner_wref)) {
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

	[[nodiscard]] std::optional<std::uint32_t> generate_handle(std::shared_ptr<Sound> owner, std::string_view file) {
		if (!is_initialized) {
			return std::nullopt;
		}

		std::filesystem::path lfile;

		if (auto const optional_file = util::fs::normalized_filepath(file); optional_file.has_value()) {
			lfile = optional_file.value();
		} else {
#ifdef AETHER_DEBUG
			errorlog("Filesystem gave an error");
#endif
			return std::nullopt;
		}

#ifdef AETHER_VERBOSE_DEBUG
		debuglog("Generating audio handle | file: \"{}\"", lfile.filename().string());
		auto const start_time = util::timer::start();
#endif

		std::uint32_t id = 1;

		while (active_sounds.find(id) != active_sounds.end()) {
			++id;
		}

		auto [iterator, _] = active_sounds.emplace(id, owner);

		if (ma_result result =
		        ma_sound_init_from_file(&engine, lfile.string().c_str(), 0, nullptr, nullptr, &iterator->second.sound);
		    result != MA_SUCCESS) {
#ifdef AETHER_DEBUG
			errorlog("Failed to initialize sound | error code: {}", static_cast<int>(result));
#endif
			active_sounds.erase(iterator);
			return std::nullopt;
		}

#ifdef AETHER_VERBOSE_DEBUG
		auto const end_time = util::timer::end(start_time);
		ma_uint64 pcm_frames;
		ma_sound_get_length_in_pcm_frames(&iterator->second.sound, &pcm_frames);
		tracelog("Successful generation | handle owner: {} | pcm frames: {}", fmt::ptr(owner.get()), pcm_frames);
		debuglog("Done | took {}ms", end_time);
#endif

		return id;
	}

	bool play(std::uint32_t id) {
		if (!is_initialized) {
			return false;
		}

		auto iterator = active_sounds.find(id);

		if (iterator == active_sounds.end()) {
			return false;
		}

		if (ma_result result = ma_sound_start(&iterator->second.sound); result != MA_SUCCESS) {
#ifdef AETHER_DEBUG
			errorlog("Failed | error code: {}", static_cast<int>(result));
#endif
			return false;
		}

		return true;
	}

	bool init() {
		if (is_initialized) {
			// already initialized
			return true;
		}

		if (ma_result result = ma_engine_init(nullptr, &engine); result != MA_SUCCESS) {
#ifdef AETHER_DEBUG
			errorlog("Couldn't initialize engine | error code: {}", static_cast<int>(result));
#endif
			return false;
		}

#ifdef AETHER_VERBOSE_DEBUG
		debuglog("Initialized");
#endif

		return is_initialized = true;
	}

	void shutdown() {
		if (!is_initialized) {
			// not yet initialized
			return;
		}

		active_sounds.clear();
		ma_engine_uninit(&engine);
		is_initialized = false;
	}

	void update() {
		if (!is_initialized) {
			return;
		}

		std::erase_if(active_sounds, [](auto& pair) {
			if (ma_sound_is_looping(&pair.second.sound)) {
				return pair.second.owner.expired();
			}

			return pair.second.owner.expired() && ma_sound_at_end(&pair.second.sound);
		});
	}

	ma_engine engine;
	std::unordered_map<std::uint32_t, scoped_sound> active_sounds;
	bool is_initialized = false;
};

// private
Audio::Audio() : impl_(std::make_unique<impl>()) {}
Audio::~Audio() = default;

std::optional<std::uint32_t> Audio::generate_handle(std::shared_ptr<Sound> owner, std::string_view file) {
	return impl_->generate_handle(owner, file);
}

bool Audio::play(std::uint32_t id) {
	return impl_->play(id);
}

// private
bool Audio::init() {
	return impl_->init();
}

// private
void Audio::shutdown() {
	impl_->shutdown();
}

// private
void Audio::update() {
	impl_->update();
}

} // namespace ae