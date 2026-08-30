#pragma once
#include <aether/mat3.hh>
#include <aether/node/components/component.hh>
#include <aether/size.hh>
#include <aether/vec2.hh>

namespace aether {

class transform final : public node_component {
public:
	transform(context const& ctx, strong_ref<node> n) noexcept;
	~transform() noexcept override;

	void set_bounds(size<uint32_t> val) noexcept;
	void set_position(vec2<float> val) noexcept;
	void set_anchor(vec2<float> val) noexcept;
	void set_scale(vec2<float> val) noexcept;
	void set_shear(vec2<float> val) noexcept;

	[[nodiscard]] mat3 matrix() const noexcept;
	[[nodiscard]] size<uint32_t> bounds() const noexcept;
	[[nodiscard]] vec2<float> position() const noexcept;
	[[nodiscard]] vec2<float> anchor() const noexcept;
	[[nodiscard]] vec2<float> scale() const noexcept;
	[[nodiscard]] vec2<float> shear() const noexcept;

protected:
	bool init_() noexcept override;
	void visit_() noexcept override;

private:
	void mark_transform_dirty_();
	void update_matrix_();

	mat3 matrix_;

	size<uint32_t> bounds_;

	vec2<float> position_;
	vec2<float> anchor_;
	vec2<float> scale_;
	vec2<float> shear_; // degrees

	vec2<bool> is_flipped_;

	float rotation_; // degrees

	bool is_transform_dirty_;
};

} // namespace aether