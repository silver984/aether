#pragma once
#include <math/vec2.hh>
#include <memory>

namespace aether {

class Scene;
class Node;

class Camera final {
	friend class Scene;

private:
	Camera(std::shared_ptr<Node>& root_node);

public:
	~Camera();

	void set_position(vec2<float> val);
	[[nodiscard]] vec2<float> position() const;

private:
	std::shared_ptr<Node>& root_node_;
	vec2<float> position_;
	float zoom_;
	float rotation_; // degrees
};

} // namespace aether