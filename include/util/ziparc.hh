#pragma once
#include <filesystem>
#include <miniz/miniz.h>
#include <string_view>
#include <util/blob.hh>

namespace aether::util {

namespace fs = std::filesystem;

class ziparc final {
public:
	ziparc() noexcept;
	ziparc(fs::path const& file) noexcept;
	~ziparc() noexcept;
	bool open(fs::path const& file) noexcept;
	bool close() noexcept;
	[[nodiscard]] bool is_open() const noexcept;
	[[nodiscard]] bool contains(std::string_view file) noexcept;
	[[nodiscard]] blob read(std::string_view file) noexcept;

private:
	mz_zip_archive archive_;
	bool is_open_;
};

} // namespace aether::util