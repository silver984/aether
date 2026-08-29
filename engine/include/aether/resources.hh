#pragma once
#include <aether/general.h>
#include <aether/loader.hh>
#include <aether/log.hh>
#include <aether/ref.hh>
#include <aether/service.hh>
#include <aether/string.hh>
#include <aether/timer.hh>
#include <aether/zip_archive.hh>
#include <utility>

namespace aether {

template <typename T>
concept has_loader = requires {
	{ loader<T>::load(std::declval<zip_archive&>(), std::declval<std::string_view>()) } -> std::same_as<strong_ref<T>>;
	{ loader<T>::unload(std::declval<T&>()) } -> std::same_as<void>;
};

class game;

template <has_loader Type>
class resources final : public service<resources<Type>> {
	friend class game;

public:
	~resources() noexcept override {
		purge_all_();
	}

	[[nodiscard]] strong_ref<Type> load(zip_archive& pkg, std::string_view file) {
		if (strong_ref<Type> from_cache = cache_fetch_(file)) {
			return from_cache;
		}

		ae_debug("Loading resource ? file: \"{}\"", file);
		timer t;
		t.start();

		strong_ref<Type> out = loader<Type>::load(pkg, file);

		if (!out) {
			ae_error("Failed to load resource ? file: \"{}\"", file);
			return nullptr;
		}

		ae_trace("Loaded resource ? address: {}", fmt::ptr(out.get()));

		t.stop();
		ae_debug("Done ({}ms)", t.duration());

		purge_unused_();
		auto [it, _] = cache_.emplace(std::string(file), std::move(out));
		return it->second;
	}

private:
	resources() {
		this->expose_();
	}

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
		ae_trace("Unloaded resource ? address: {}", fmt::ptr(&data));
	}

	string_map<strong_ref<Type>> cache_;
};

} // namespace aether