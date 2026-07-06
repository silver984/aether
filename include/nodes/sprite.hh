#pragma once
#include <math/rect.hh>
#include <nodes/node.hh>

namespace aether {

class texture2d;

class sprite : public node {
public:
	enum class texture_wrap : int {
		CLAMP,
		REPEAT,
		MIRROR_CLAMP,
		MIRROR_REPEAT
	};

	struct descriptor final {
		std::string_view file;
		texture_wrap wrap_type = texture_wrap::CLAMP;
		bool has_antialiasing  = true;
	};

	sprite(context const& ctx, descriptor const& desc);
	~sprite() override;

	void toggle_antialiasing(bool val) const;
	bool set_texture(std::string_view file);
	void set_texture_wrap(texture_wrap type);
	void set_texture_source_rect(rect<float> const& val); // todo: pos, bounds, x, y, width, height
	void update_bounds();
	[[nodiscard]] rect<float> texture_source_rect() const;

protected:
	bool init_() override;
	void draw_(mat3 const& transform, rgba color) override;

private:
	ref<texture2d> texture_;
	std::string const file_arg_;
	texture_wrap const wrap_type_arg_;
	rect<float> texture_source_rect_;
	bool const has_antialiasing_arg_;
};

} // namespace aether