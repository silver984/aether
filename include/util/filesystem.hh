#pragma once
#include <data/u8vec.hh>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>

namespace aether::util {

[[nodiscard]] std::string file_extension(std::filesystem::path const& file);
[[nodiscard]] u8vec read_file_to_buffer(std::filesystem::path const& file);

} // namespace aether::util