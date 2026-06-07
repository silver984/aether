#pragma once
#include <byte_buffer.hh>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>

namespace aether::util {

[[nodiscard]] std::string file_extension(std::filesystem::path const& file);
[[nodiscard]] byte_buffer read_file_to_byte_buffer(std::filesystem::path const& file);

} // namespace aether::util