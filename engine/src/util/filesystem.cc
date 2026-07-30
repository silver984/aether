#include <algorithm>
#include <cstdint>
#include <debug/log.hh>
#include <fstream>
#include <util/filesystem.hh>

namespace aether::util {

std::string file_extension(std::filesystem::path const& file) {
	if (!file.has_extension()) {
		return {};
	}

	std::string extension = file.extension().string();
	std::transform(extension.begin(), extension.end(), extension.begin(), [](uint8_t c) {
		return std::tolower(c);
	});

	return extension;
}

blob read_file_to_buffer(std::filesystem::path const& file) {
	std::ifstream lfile(file, std::ios::binary | std::ios::ate);

	if (!lfile.is_open()) {
		return {};
	}

	std::streamsize size = lfile.tellg();
	lfile.seekg(0, std::ios::beg);
	blob out(size);

	if (!lfile.read(reinterpret_cast<char*>(out.data()), size)) {
		return {};
	}

	return out;
}

} // namespace aether::util