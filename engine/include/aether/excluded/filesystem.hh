#pragma once
#include <aether/blob.hh>
#include <filesystem>
#include <string>

namespace aether {

[[nodiscard]] std::string file_extension(std::filesystem::path const& file);
[[nodiscard]] blob read_file_to_buffer(std::filesystem::path const& file);

} // namespace aether