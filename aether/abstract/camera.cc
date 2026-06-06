#include <abstract/camera.hh>
#include <objects/node.hh>

namespace aether {

// private
camera::camera(std::shared_ptr<node>& root_node)
    : root_node_(root_node) {}

camera::~camera() = default;

void camera::set_position(vec2<float> val) {
	if (position_ == val) {
		return;
	}

	position_ = val;
	root_node_->mark_transform_dirty();
}

vec2<float> camera::position() const {
	return position_;
}

} // namespace aether