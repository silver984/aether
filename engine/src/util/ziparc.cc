#include <cstddef>
#include <cstring>
#include <util/ziparc.hh>

namespace aether::util {

ziparc::ziparc() noexcept
        : archive_()
        , is_open_(false) {
}

ziparc::ziparc(fs::path const& file)
        : archive_()
        , is_open_(false) {
	open(file);
}

ziparc::~ziparc() {
	close();
}

bool ziparc::open(fs::path const& file) {
	if (!fs::exists(file) || is_open_) {
		return false;
	}
	is_open_ = mz_zip_reader_init_file(&archive_, file.string().c_str(), 0) == MZ_TRUE;
	return is_open_;
}

bool ziparc::close() {
	if (!is_open_) {
		return false;
	}

	bool closed = mz_zip_reader_end(&archive_) == MZ_TRUE;

	if (closed) {
		is_open_ = false;
	}

	return closed;
}

bool ziparc::is_open() const noexcept {
	return is_open_;
}

bool ziparc::contains(std::string_view file) {
	if (!is_open_) {
		return false;
	}
	constexpr int invalid_index = -1;
	return mz_zip_reader_locate_file(&archive_, file.data(), nullptr, 0) != invalid_index;
}

blob ziparc::read(std::string_view file) {
	if (!contains(file)) {
		return {};
	}

	size_t size = 0;
	void* mem   = mz_zip_reader_extract_file_to_heap(&archive_, file.data(), &size, 0);

	if (!mem) {
		return {};
	}

	blob bytes(size);
	std::memcpy(bytes.data(), mem, size);
	mz_free(mem);

	return bytes;
}

} // namespace aether::util