#include <cstddef>
#include <util/ziparc.hh>

namespace aether::util {

ziparc::ziparc() noexcept
        : archive_() {
}

ziparc::ziparc(fs::path const& file) noexcept
        : archive_() {
	open(file);
}

ziparc::~ziparc() noexcept {
	close();
}

bool ziparc::open(fs::path const& file) noexcept {
	if (!fs::exists(file)) {
		return false;
	}
	return mz_zip_reader_init_file(&archive_, file.string().data(), 0) == MZ_TRUE;
}

bool ziparc::close() noexcept {
	return mz_zip_reader_end(&archive_) == MZ_TRUE;
}

bool ziparc::contains(std::string_view file) noexcept {
	constexpr int invalid_index = -1;
	return mz_zip_reader_locate_file(&archive_, file.data(), nullptr, 0) != invalid_index;
}

blob ziparc::read(std::string_view file) noexcept {
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