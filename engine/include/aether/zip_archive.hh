#pragma once
#include <aether/blob.hh>
#include <aether/ref.hh>
#include <string_view>

namespace aether {

class zip_archive final {
public:
	zip_archive();
	explicit zip_archive(std::string_view file);
	~zip_archive();

	bool open(std::string_view file) const;
	bool close() const;
	[[nodiscard]] bool is_open() const;
	[[nodiscard]] bool contains(std::string_view file) const;
	[[nodiscard]] blob read(std::string_view file) const;

private:
	struct impl;
	unique_ref<impl> impl_;
};

} // namespace aether