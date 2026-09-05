#pragma once
#include <aether/general.h>
#include <aether/loader.hh>
#include <aether/log.hh>
#include <aether/ref.hh>
#include <aether/string.hh>
#include <aether/timer.hh>
#include <aether/zip_archive.hh>

#include <utility>

namespace aether::_res_impl {

template <typename T>
concept loadable_ = requires {
	{ loader<T>::load(std::declval<zip_archive const&>(), std::declval<std::string_view>()) } -> std::same_as<strong_ref<T>>;
	{ loader<T>::unload(std::declval<T const&>()) } -> std::same_as<void>;
};

} // namespace aether::_res_impl

namespace aether {

class game;

template <_res_impl::loadable_ T>
class resources final {
	friend class game;

public:
	~resources() { purge_all_(); }

	[[nodiscard]] strong_ref<T> load(zip_archive const& pkg, std::string_view file) {
		if (strong_ref<T> from_cache = cache_fetch_(file)) {
			return from_cache;
		}

		ae_trace("Loading resource ? file: \"{}\"", file);
		timer t;
		t.start();

		strong_ref<T> out = loader<T>::load(pkg, file);

		if (!out) {
			ae_error("Failed to load resource ? file: \"{}\"", file);
			return nullptr;
		}

		t.stop();
		ae_trace("Done ({}ms) ? address: {}", t.duration(), fmt::ptr(out.get()));

		purge_unused_();
		auto [it, _] = cache_.emplace(std::string(file), std::move(out));
		return it->second;
	}

private:
	resources() = default;

	[[nodiscard]] strong_ref<T> cache_fetch_(std::string_view file) const {
		if (auto it = cache_.find(file); it != cache_.end()) {
			return it->second;
		}
		return nullptr;
	}

	void purge_unused_() {
		for (auto it = cache_.begin(); it != cache_.end();) {
			if (it->second.strong_count() <= 1) {
				unload_(*it->second);
				it = cache_.erase(it);
				continue;
			}
			++it;
		}
	}

	void purge_all_() {
		for (auto it = cache_.begin(); it != cache_.end();) {
			unload_(*it->second);
			it = cache_.erase(it);
		}
	}

	void unload_(T& data) {
		loader<T>::unload(data);
		ae_trace("Unloaded resource ? address: {}", fmt::ptr(&data));
	}

	string_map<strong_ref<T>> cache_;
};

} // namespace aether