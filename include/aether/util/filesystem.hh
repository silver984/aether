#pragma once
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>

namespace ae::fs {

std::optional<std::filesystem::path> normalized_filepath(std::string_view str);
std::string file_extension(std::filesystem::path const& file_path);

} // namespace ae::fs