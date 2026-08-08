#include <aether/zip_archive.hh>
#include <cstddef>
#include <cstring>
#include <filesystem>

namespace aether {

namespace fs = std::filesystem;

zip_archive::zip_archive() noexcept
        : archive_()
        , is_open_(false) {
}

zip_archive::zip_archive(char const* pkg) noexcept
        : archive_()
        , is_open_(false) {
	(void)open(pkg);
}

zip_archive::~zip_archive() noexcept {
	(void)close();
}

bool zip_archive::open(std::string_view pkg) noexcept {
	if (!fs::exists(pkg) || is_open_) {
		return false;
	}
	is_open_ = (mz_zip_reader_init_file(&archive_, pkg.data(), 0) == MZ_TRUE);
	return is_open_;
}

bool zip_archive::close() noexcept {
	if (!is_open_) {
		return false;
	}
	bool closed = mz_zip_reader_end(&archive_) == MZ_TRUE;
	if (closed) {
		is_open_ = false;
	}
	return closed;
}

bool zip_archive::is_open() const noexcept {
	return is_open_;
}

bool zip_archive::contains(std::string_view file) {
	if (!is_open_) {
		return false;
	}
	constexpr int INVALID_INDEX = -1;
	return (mz_zip_reader_locate_file(&archive_, file.data(), nullptr, 0) != INVALID_INDEX);
}

blob zip_archive::read(std::string_view file) {
	if (!contains(file)) {
		return {};
	}

	size_t size = 0;
	void* mem   = mz_zip_reader_extract_file_to_heap(&archive_, file.data(), &size, 0);

	if (!mem) {
		return {};
	}

	blob bytes(size);
	(void)std::memcpy(bytes.data(), mem, size);
	mz_free(mem);

	return bytes;
}

} // namespace aether