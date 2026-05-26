#include <aether/debug/log.hh>
#include <aether/systems/repos/TextureRepo.hh>
#include <aether/util/filesystem.hh>

namespace ae {

TextureRepo::TextureRepo()  = default;
TextureRepo::~TextureRepo() = default;

std::shared_ptr<Texture> TextureRepo::fetch(std::string_view file) {
	std::filesystem::path file_path;

	if (auto opt_path = fs::normalized_filepath(file); opt_path.has_value()) {
		file_path = opt_path.value();
	} else {
		errorlog("Failed");
		return nullptr;
	}

	if (!std::filesystem::exists(file_path)) {
		errorlog("File doesn't exist | file: \"{}\"", file_path.generic_string());
		return nullptr;
	}

	// TODO: support other formats than just PNG
	if (auto const file_extension = fs::file_extension(file_path); file_extension != "png") {
		errorlog("Unsupported file format | file: \"{}\"", file_path.generic_string());
		return nullptr;
	}
}

} // namespace ae