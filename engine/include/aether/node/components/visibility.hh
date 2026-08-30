#pragma once
#include <aether/node/components/component.hh>

namespace aether {

class visibility final : public node_component {
public:
	using node_component::node_component;
	inline void set_visibile(bool val) noexcept { val_ = val; }
	[[nodiscard]] inline bool is_visible() noexcept { return val_; }

private:
	bool val_ = true;
};

} // namespace aether