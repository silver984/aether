#include <camera.hh>
#include <nodes/node.hh>

namespace aether {

camera::camera(std::shared_ptr<node>& root)
        : root_node_(root) {}

camera::~camera() = default;

void camera::set_position(vec2<float> val) {
	if (position_ == val) {
		return;
	}

	position_ = val;
	// root_node_->mark_transform_dirty_();
}

vec2<float> camera::position() const {
	return position_;
}

} // namespace aether