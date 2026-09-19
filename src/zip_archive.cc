#include <aether/zip_archive.hh>

#include <miniz/miniz.h>

#include <cstddef>
#include <cstring>
#include <filesystem>

namespace aether {

namespace fs = std::filesystem;

struct zip_archive::impl_ final {
	bool open_(std::string_view file) {
		if (!fs::exists(file) || is_open_) {
			return false;
		}
		is_open_ = mz_zip_reader_init_file(&archive_, file.data(), 0) == MZ_TRUE;
		return is_open_;
	}

	bool close_() {
		if (!is_open_) {
			return false;
		}
		bool closed = mz_zip_reader_end(&archive_) == MZ_TRUE;
		if (closed) {
			is_open_ = false;
		}
		return closed;
	}

	bool contains_(std::string_view file) {
		if (!is_open_) {
			return false;
		}
		constexpr int INVALID_INDEX = -1;
		return mz_zip_reader_locate_file(&archive_, file.data(), nullptr, 0) != INVALID_INDEX;
	}

	blob read_(std::string_view file) {
		if (!contains_(file)) {
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

	mz_zip_archive archive_;
	bool is_open_;
};

zip_archive::zip_archive()
        : ptr_impl_(unique_ref<impl_>::create()) {}

zip_archive::zip_archive(std::string_view file)
        : ptr_impl_(unique_ref<impl_>::create()) {
	open(file);
}

zip_archive::~zip_archive() { close(); }

bool zip_archive::open(std::string_view file) const { return ptr_impl_->open_(file); }
bool zip_archive::close() const { return ptr_impl_->close_(); }
bool zip_archive::is_open() const { return ptr_impl_->is_open_; }
bool zip_archive::contains(std::string_view file) const { return ptr_impl_->contains_(file); }

blob zip_archive::read(std::string_view file) const { return ptr_impl_->read_(file); }

} // namespace aether