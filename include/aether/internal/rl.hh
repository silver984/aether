#pragma once
#include <aether/math/size.hh>
#include <aether/graphics/texture.hh>
#include <optional>

namespace ae::rl {

bool init_window(char const* title, size<int> const& resolution, int fps);
[[nodiscard]] float delta_time();
[[nodiscard]] std::optional<texture> load_texture(char const* file);

}