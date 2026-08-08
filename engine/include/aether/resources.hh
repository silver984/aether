#pragma once
#include <aether/general.h>
#include <aether/loader.hh>
#include <aether/log.hh>
#include <aether/ref.hh>
#include <aether/string.hh>
#include <aether/timer.hh>
#include <aether/zip_archive.hh>
#include <utility>

namespace aether {

class game;

}

namespace aether::core {

template <typename Type>
class resources final {
	friend class aether::game;

public:
	~resources() noexcept {
		purge_all_();
	}

	DELETE_COPY_AND_MOVE(resources);

	[[nodiscard]] strong_ref<Type> load(zip_archive& pkg, std::string_view file) {
		if (strong_ref<Type> from_cache = cache_fetch_(file)) {
			return from_cache;
		}

		if (!pkg.contains(file)) {
			AETHER_ENGINE_ERRORLOG("\"{}\" does not exist from the specified resource package", file);
			return nullptr;
		}

		AETHER_ENGINE_DEBUGLOG("Loading resource ? file: \"{}\"", file);
		timer t;
		t.start();

		blob buffer = pkg.read(file);

		if (buffer.empty()) {
			AETHER_ENGINE_ERRORLOG("Failed to read buffer ? file: \"{}\"", file);
			return nullptr;
		}

		AETHER_ENGINE_TRACELOG("Read buffer ? size: {:.2f}mib", buffer.size() / (1024.f * 1024.f));

		strong_ref<Type> out = loader<Type>::load(file, buffer);

		if (!out) {
			AETHER_ENGINE_ERRORLOG("Failed to load resource ? file: \"{}\"", file);
			return nullptr;
		}

		AETHER_ENGINE_TRACELOG("Loaded resource ? address: {}", fmt::ptr(out.get()));

		t.stop();
		AETHER_ENGINE_DEBUGLOG("Done ({}ms)", t.duration());

		purge_unused_();
		auto [it, _] = cache_.emplace(std::string(file), std::move(out));
		return it->second;
	}

private:
	resources() noexcept = default;

	[[nodiscard]] strong_ref<Type> cache_fetch_(std::string_view file) const {
		if (auto it = cache_.find(file); it != cache_.end()) {
			return it->second;
		}
		return nullptr;
	}

	void purge_unused_() noexcept {
		for (auto it = cache_.begin(); it != cache_.end();) {
			if (it->second.strong_count() <= 1) {
				unload_(*it->second);
				it = cache_.erase(it);
				continue;
			}
			++it;
		}
	}

	void purge_all_() noexcept {
		for (auto it = cache_.begin(); it != cache_.end();) {
			unload_(*it->second);
			it = cache_.erase(it);
		}
	}

	void unload_(Type& data) {
		loader<Type>::unload(data);
		AETHER_ENGINE_TRACELOG("Unloaded resource ? address: {}", fmt::ptr(&data));
	}

	string_map<strong_ref<Type>> cache_;
};

} // namespace aether::core