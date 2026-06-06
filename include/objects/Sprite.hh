#pragma once
#include <math/rect.hh>
#include <objects/node.hh>

struct Texture;
using rltexture = Texture;

namespace aether {

class sprite : public node {
public:
	enum class texture_wrap : int { clamp, repeat, mirror_clamp, mirror_repeat };

	struct descriptor final {
		std::string_view file;
		texture_wrap wrap_type = texture_wrap::clamp;
		bool has_antialiasing  = true;
	};

	sprite(context const& ctx, descriptor const& desc);
	~sprite() override;

	void toggle_antialiasing(bool val) const;
	bool set_texture(std::string_view file);
	void set_texture_wrap(texture_wrap type);
	void set_texture_source_rect(rect<int> val); // TODO: pos, bounds, x, y, width, height
	void update_bounds();
	[[nodiscard]] rect<int> texture_source_rect() const;

protected:
	bool init() override;
	void draw(mat3 const& transform, rgba color) override;

private:
	std::shared_ptr<rltexture> texture_;
	std::string const file_arg_;
	texture_wrap const wrap_type_arg_;
	rect<float> texture_source_rect_;
	bool const has_antialiasing_arg_;
};

} // namespace aether