#pragma once
#include <loader.hh>
#include <util/ref.hh>
#include <util/string.hh>
#include <util/ziparc.hh>
#include <utility>

namespace aether {

class game;

template <typename Type>
class resources final {
	friend class game;

public:
	~resources() noexcept = default;

	[[nodiscard]] ref<Type> load(util::ziparc& archive, std::string_view file) {
		if (ref<Type> from_cache = cache_fetch_(file)) {
			return from_cache;
		}

		if (!archive.contains(file)) {
			return nullptr;
		}

		blob buffer = archive.read(file);

		if (buffer.empty()) {
			return nullptr;
		}

		ref<Type> out = loader<Type>::load(buffer);

		if (!out) {
			return nullptr;
		}

		purge_unused_();
		auto [it, _] = cache_.emplace(std::string(file), std::move(out));
		return it->second;
	}

private:
	resources() noexcept {
		purge_all_();
	}

	[[nodiscard]] ref<Type> cache_fetch_(std::string_view file) const {
		if (auto it = cache_.find(file); it != cache_.end()) {
			return it->second;
		}
		return nullptr;
	}

	void purge_unused_() noexcept {
		for (auto it = cache_.begin(); it != cache_.end();) {
			ref<Type>& data = it->second;
			if (data.strong_count() <= 1) {
				loader<Type>::unload(data);
				it = cache_.erase(it);
				continue;
			}
			++it;
		}
	}

	void purge_all_() noexcept {
		for (auto it = cache_.begin(); it != cache_.end();) {
			ref<Type>& data = it->second;
			loader<Type>::unload(data);
			it = cache_->erase(it);
		}
	}

	util::stringmap<ref<Type>> cache_;
};

} // namespace aether