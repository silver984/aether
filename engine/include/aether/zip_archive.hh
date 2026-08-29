#pragma once
#include <aether/blob.hh>
#include <miniz/miniz.h>
#include <string_view>

namespace aether {

class zip_archive final {
public:
	zip_archive() noexcept;
	explicit zip_archive(std::string_view file);
	~zip_archive() noexcept;

	bool open(std::string_view file) noexcept;
	bool close() noexcept;
	[[nodiscard]] bool is_open() const noexcept;
	[[nodiscard]] bool contains(std::string_view file);
	[[nodiscard]] blob read(std::string_view file);

private:
	mz_zip_archive archive_;
	bool is_open_;
};

} // namespace aether