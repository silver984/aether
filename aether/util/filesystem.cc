#include <aether/debug/log.hh>
#include <aether/util/filesystem.hh>
#include <algorithm>
#include <cstdint>

namespace ae::util::fs {

std::optional<std::filesystem::path> normalized_filepath(std::string_view str) {
	try {
		return std::filesystem::canonical(str);
	} catch (std::filesystem::filesystem_error const& err) {
		errorlog("Caught filesystem error | arg: \"{}\" | what: {}", str, err.what());
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

bool file_extension_matches_any(std::string_view extension, std::initializer_list<std::string_view> candidates) {
	return std::find(candidates.begin(), candidates.end(), extension) != candidates.end();
}

} // namespace ae::util::fs