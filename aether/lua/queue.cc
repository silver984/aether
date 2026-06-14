#include <lua/queue.hh>
#include <vector>

namespace {

using fnvec = std::vector<std::function<void(sol::state_view&)>>;
fnvec& bindings() {
	static fnvec instance;
	return instance;
}

} // namespace

namespace aether::lua::impl_ {

void queue_binding_(std::function<void(sol::state_view&)>&& cb) {
	bindings().emplace_back(std::move(cb));
}

void run_bindings_(sol::state_view lua) {
	auto& lbindings = bindings();
	for (auto const& cb : lbindings) {
		cb(lua);
	}
	lbindings.clear();
}

} // namespace aether::lua::impl_