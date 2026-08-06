#pragma once
#include <aether/filesystem.hh>
#include <aether/log.hh>
#include <aether/ref.hh>
#include <aether/string.hh>
#include <aether/timer.hh>
#include <filesystem>
#include <string_view>
#include <unordered_map>

namespace aether {

namespace fs = std::filesystem;

template <typename Type>
class cache {
public:
	cache() noexcept          = default;
	virtual ~cache() noexcept = default;

	[[nodiscard]] strong_ref<Type> fetch(std::string_view file) {
		fs::path canonical_file = fs::weakly_canonical(file);

		if (!fs::exists(canonical_file)) {
			AETHER_ENGINE_ERRORLOG("File doesn't exist ? file: \"{}\"", file);
			return nullptr;
		}

		if (strong_ref<Type> from_bank = bank_fetch_(canonical_file)) {
			return from_bank;
		}

		purge_unused();

		AETHER_ENGINE_DEBUGLOG("Loading resource ? file: \"{}\"", file);
		util::timer t;
		t.start();

		strong_ref<Type> resource = load_(canonical_file);

		if (!resource) {
			AETHER_ENGINE_ERRORLOG("Failed to load resource ? file: \"{}\"", file);
			return nullptr;
		}

		t.stop();
		AETHER_ENGINE_DEBUGLOG("Done ({}ms)", t.duration());

		auto const [it, _] = bank_.emplace(canonical_file, std::move(resource));
		return it->second;
	}

	void purge_unused() {
		std::erase_if(bank_, [](auto const& pair) {
			return pair.second.strong_count() <= 1;
		});
	}

protected:
	[[nodiscard]] virtual strong_ref<Type> load_(fs::path const& file) = 0;

	void clear_() noexcept {
		bank_.clear();
	}

private:
	[[nodiscard]] strong_ref<Type> bank_fetch_(fs::path const& file) const {
		if (auto const it = bank_.find(file); it != bank_.end()) {
			return it->second;
		}
		return nullptr;
	}

	std::unordered_map<fs::path, strong_ref<Type>> bank_;
};

} // namespace aether