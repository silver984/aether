#pragma once
#include <aether/node/components/component.hh>

namespace aether {

class visibility final : public node_component {
public:
	using node_component::node_component;
	void set_visibile(bool val) { val_ = val; }
	[[nodiscard]] bool is_visible() { return val_; }

private:
	bool val_ = true;
};

} // namespace aether