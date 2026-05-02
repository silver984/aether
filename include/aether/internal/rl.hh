#pragma once
#include <aether/math/size.hh>

namespace ae::rl {

bool init_window(char const* title, size<int> const& resolution, int fps);
[[nodiscard]] float delta_time();

}