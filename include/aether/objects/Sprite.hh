#pragma once
#include <aether/math_types/rect.hh>
#include <aether/objects/Node.hh>

struct Texture;

namespace ae {

enum class texture_wrap : int { clamp, repeat, mirror_clamp, mirror_repeat };

class Sprite : public NodeIdentity<Sprite> {
	friend class ae::NodeIdentity<Sprite>;

public:
	Sprite(Context const& ctx, std::string_view file);
	~Sprite() override;

	void toggle_antialiasing(bool val) const;
	bool set_texture(std::string_view file);
	void set_texture_wrap(texture_wrap type);
	void set_texture_source_rect(rect<int> val, bool update_bounds = false); // TODO: pos, bounds, x, y, width, height
	[[nodiscard]] rect<int> texture_source_rect() const;

protected:
	bool init() override;
	void draw(mat3 const& transform, rgba color) override;

private:
	static constexpr std::string_view TYPE_ = "Sprite";
	std::shared_ptr<Texture> texture_;
	rect<int> texture_source_rect_;
	std::string const file_arg_;
};

} // namespace ae