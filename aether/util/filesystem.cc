#ifdef AETHER_DEBUG
	#include <log.hh>
#endif
#include <algorithm>
#include <cstdint>
#include <util/filesystem.hh>

namespace aether::util::fs {

std::optional<std::filesystem::path> normalized_filepath(std::string_view str) {
	try {
		return std::filesystem::canonical(str);
	} catch (std::filesystem::filesystem_error const& err) {
#ifdef AETHER_DEBUG
		errorlog("Caught filesystem error | arg: \"{}\" | what: {}", str, err.what());
#endif
		return std::nullopt;
	}
}

std::string file_extension(std::filesystem::path const& file_path) {
	if (!file_path.has_extension()) {
		return {};
	}

	auto extension = file_path.extension().string();
	std::transform(extension.begin(), extension.end(), extension.begin(), [](std::uint8_t c) {
		return std::tolower(c);
	});

	return extension;
}

} // namespace aether::util::fs