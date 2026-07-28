#pragma once
#include <debug/log.hh>
#include <loader.hh>
#include <util/ref.hh>
#include <util/string.hh>
#include <util/timer.hh>
#include <util/ziparc.hh>
#include <utility>

namespace aether {

class game;

template <typename Type>
class resources final {
	friend class game;

public:
	~resources() noexcept {
		purge_all_();
	}

	[[nodiscard]] strong_ref<Type> load(util::ziparc& archive, std::string_view file) {
		if (strong_ref<Type> from_cache = cache_fetch_(file)) {
			return from_cache;
		}

		if (!archive.contains(file)) {
			return nullptr;
		}

		AETHER_DEBUGLOG("Loading resource ? file: \"{}\"", file);
		util::timer t;
		t.start();

		blob buffer = archive.read(file);

		if (buffer.empty()) {
			AETHER_ERRORLOG("Failed to read buffer ? file: \"{}\"", file);
			return nullptr;
		}

		AETHER_TRACELOG("Read buffer ? size: {}", buffer.size());

		strong_ref<Type> out = loader<Type>::load(buffer);

		if (!out) {
			AETHER_ERRORLOG("Failed to load resource ? file: \"{}\"", file);
			return nullptr;
		}

		AETHER_TRACELOG("Loaded resource ? address: {}", fmt::ptr(out.get()));

		t.stop();
		AETHER_DEBUGLOG("Done ({}ms)", t.duration());

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
		AETHER_TRACELOG("Unloaded resource ? address: {}", fmt::ptr(&data));
	}

	util::stringmap<strong_ref<Type>> cache_;
};

} // namespace aether