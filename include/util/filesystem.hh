#pragma once
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>

namespace aether::util::fs {

[[nodiscard]] std::optional<std::filesystem::path> normalized_filepath(std::string_view str);
[[nodiscard]] std::string file_extension(std::filesystem::path const& file_path);

} // namespace aether::util::fs