#pragma once
#include <aether/mat3.hh>
#include <aether/node/components/component.hh>
#include <aether/size.hh>
#include <aether/vec2.hh>

namespace aether {

class transform final : public node_component {
public:
	using node_component::node_component;

	void set_bounds(size<uint32_t> val) noexcept;

	void set_position(vec2<float> val) noexcept;
	void set_anchor(vec2<float> val) noexcept;
	void set_scale(vec2<float> val) noexcept;
	void set_shear(vec2<float> val) noexcept;

	void set_flipped(vec2<bool> val) noexcept;

	void set_rotation(float val) noexcept;

	[[nodiscard]] inline mat3 matrix() const noexcept { return matrix_; }

	[[nodiscard]] inline size<uint32_t> bounds() const noexcept { return bounds_; }
	[[nodiscard]] inline uint32_t width() const noexcept { return bounds_.width; }
	[[nodiscard]] inline uint32_t height() const noexcept { return bounds_.height; }

	[[nodiscard]] inline vec2<float> position() const noexcept { return position_; }
	[[nodiscard]] inline vec2<float> anchor() const noexcept { return anchor_; }
	[[nodiscard]] inline vec2<float> scale() const noexcept { return scale_; }
	[[nodiscard]] inline vec2<float> shear() const noexcept { return shear_; }

	[[nodiscard]] inline vec2<bool> is_flipped() const noexcept { return is_flipped_; }

	[[nodiscard]] inline float rotation() const noexcept { return rotation_; }

protected:
	void node_pushed_() noexcept override;
	void visit_() noexcept override;

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