#pragma once
#include <aether/node/components/component.hh>
#include <algorithm>

namespace aether {

class time final : public node_component {
public:
	using node_component::node_component;
	inline void set_scale(float val) { scale_ = std::max(val, 0.f); }
	[[nodiscard]] inline float scale() const { return scale_; }

private:
	float scale_ = 1.f;
};

} // namespace aether