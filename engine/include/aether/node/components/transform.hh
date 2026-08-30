#pragma once
#include <aether/mat3.hh>
#include <aether/node/components/component.hh>
#include <aether/size.hh>
#include <aether/vec2.hh>

namespace aether {

class transform final : public node_component {
public:
	using node_component::node_component;

	void set_bounds(size<uint32_t> val);

	void set_position(vec2<float> val);
	void set_anchor(vec2<float> val);
	void set_scale(vec2<float> val);
	void set_shear(vec2<float> val);

	void set_flipped(vec2<bool> val);

	void set_rotation(float val);

	[[nodiscard]] mat3 matrix() const { return matrix_; }

	[[nodiscard]] size<uint32_t> bounds() const { return bounds_; }
	[[nodiscard]] uint32_t width() const { return bounds_.width; }
	[[nodiscard]] uint32_t height() const { return bounds_.height; }

	[[nodiscard]] vec2<float> position() const { return position_; }
	[[nodiscard]] vec2<float> anchor() const { return anchor_; }
	[[nodiscard]] vec2<float> scale() const { return scale_; }
	[[nodiscard]] vec2<float> shear() const { return shear_; }

	[[nodiscard]] vec2<bool> is_flipped() const { return is_flipped_; }

	[[nodiscard]] float rotation() const { return rotation_; }

protected:
	void node_pushed_() override;
	void visit_() override;

private:
	void mark_transform_dirty_();
	void update_matrix_();

	mat3 matrix_ = mat3::identity();

	size<uint32_t> bounds_;

	vec2<float> position_;
	vec2<float> anchor_ = vec2<float>(0.5f);
	vec2<float> scale_  = vec2<float>(1.f);
	vec2<float> shear_; // degrees

	vec2<bool> is_flipped_ = vec2<bool>(false);

	float rotation_ = 0.f; // degrees

	bool is_transform_dirty_ = false;
};

} // namespace aether