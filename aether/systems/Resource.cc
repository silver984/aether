#include <aether/systems/Resource.hh>
#include <raylib.h>
#include <filesystem>
#include <string>

namespace {

struct file_path final {
	static file_path parse(std::string_view file) {
		std::filesystem::path abs = std::filesystem::absolute(file);
		std::string ext = abs.extension().string();

		if (!ext.empty() && ext[0] == '.') {
			// remove the dot from the extension
			ext.erase(0, 1);
		}

		// make extension lowercase
		std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

		return {
			.str = abs.string(),
			.dir = abs.parent_path().string(),
			.name = abs.stem().string(),
			.ext = ext
		};
	}

	std::string str;
	std::string dir;
	std::string name;
	std::string ext;
};

}

namespace ae {

// private
Resource::Resource() = default;

// private
Resource::~Resource() = default;

sptr<texture> Resource::load_texture(std::string_view file) {
	file_path path = file_path::parse(file);

	// TODO: check format validity

	if (
		auto it = textures_.find(path.str);
		it != textures_.end()
	) {
		return it->second;
	}

	return sptr<texture>();
}

}