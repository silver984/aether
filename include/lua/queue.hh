#pragma once
#include <functional>
#include <sol/state_view.hpp>

namespace aether::lua::impl_ {

void queue_binding_(std::function<void(sol::state_view&)>&& cb);
void run_bindings_(sol::state_view lua);

} // namespace aether::lua