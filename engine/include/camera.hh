#pragma once
#include <math/vec2.hh>
#include <memory>

namespace aether {

class scene;
class node;

class camera final {
	friend class scene;

public:
	~camera();
	void set_position(vec2<float> val);
	[[nodiscard]] vec2<float> position() const;

private:
	camera(std::shared_ptr<node>& root);

	std::shared_ptr<node>& root_node_;
	vec2<float> position_;
	float zoom_;
	float rotation_; // degrees
};

} // namespace aether