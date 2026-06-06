#include <objects/Node.hh>
#include <objects/abstract/Camera.hh>

namespace aether {

// private
Camera::Camera(std::shared_ptr<Node>& root_node)
    : root_node_(root_node) {}

Camera::~Camera() = default;

void Camera::set_position(vec2<float> val) {
	if (position_ == val) {
		return;
	}

	position_ = val;
	root_node_->mark_transform_dirty();
}

vec2<float> Camera::position() const {
	return position_;
}

} // namespace aether