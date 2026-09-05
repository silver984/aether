#pragma once
#include <aether/size.hh>

#include <cstdint>
#include <string_view>

namespace aether::_window_impl {

void try_init_(std::string_view title, size<uint32_t> bounds, uint32_t fps);
void close_();
[[nodiscard]] bool should_close_();
[[nodiscard]] bool is_minimized_();

} // namespace aether::_window_impl

namespace aether::window {

[[nodiscard]] size<uint32_t> bounds();
[[nodiscard]] uint32_t fps();

} // namespace aether::window