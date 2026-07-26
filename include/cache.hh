#pragma once
#include <debug/log.hh>
#include <filesystem>
#include <string_view>
#include <unordered_map>
#include <util/filesystem.hh>
#include <util/ref.hh>
#include <util/string.hh>
#include <util/timer.hh>

namespace aether {

namespace fs = std::filesystem;

template <typename T>
class cache {
public:
	cache(util::stringset&& valid_formats) noexcept
	        : valid_formats_(valid_formats) {
	}
	virtual ~cache() noexcept = default;

	[[nodiscard]] ref<T> fetch(std::string_view file) {
		fs::path canonical_file = fs::weakly_canonical(file);

		if (!fs::exists(canonical_file)) {
			AETHER_ERRORLOG("File doesn't exist ? file: \"{}\"", file);
			return nullptr;
		}

		if (ref<T> from_bank = bank_fetch_(canonical_file)) {
			AETHER_TRACELOG("{}", (from_bank == nullptr));
			return from_bank;
		}

		if (!is_file_format_valid_(canonical_file)) {
			return nullptr;
		}

		purge_unused();

		AETHER_DEBUGLOG("Loading resource ? file: \"{}\"", file);
		util::timer t;
		t.start();

		ref<T> resource = load_(canonical_file);

		if (!resource) {
			AETHER_ERRORLOG("Failed to load resource ? file: \"{}\"", file);
			return nullptr;
		}

		t.stop();
		AETHER_DEBUGLOG("Done ({}ms)", t.duration());

		auto const [it, _] = bank_.emplace(canonical_file, std::move(resource));
		return it->second;
	}

	void purge_unused() {
		std::erase_if(bank_, [](auto const& pair) {
			return pair.second.strong_count() <= 1;
		});
	}

protected:
	[[nodiscard]] virtual ref<T> load_(fs::path const& file) = 0;

	void clear_() noexcept {
		bank_.clear();
	}

private:
	[[nodiscard]] ref<T> bank_fetch_(fs::path const& file) const {
		if (auto const it = bank_.find(file); it != bank_.end()) {
			return it->second;
		}
		return nullptr;
	}

	[[nodiscard]] bool is_file_format_valid_(fs::path const& file) const {
		std::string const ext = util::file_extension(file);
		return std::find(valid_formats_.begin(), valid_formats_.end(), ext) != valid_formats_.end();
	}

	std::unordered_map<fs::path, ref<T>> bank_;
	util::stringset const valid_formats_;
};

} // namespace aether