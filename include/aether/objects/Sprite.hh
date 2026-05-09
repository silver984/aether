#ifndef __AETHER_OBJECTS_SPRITE_HH__
#define __AETHER_OBJECTS_SPRITE_HH__

#include <aether/objects/Node.hh>
#include <aether/math/rect.hh>
#include <memory>
#include <string_view>
#include <string>

struct Texture;

namespace ae {

enum class texture_wrap : int {
	clamp, repeat, mirror_clamp, mirror_repeat
};

class Resource;

class Sprite : public Node {
public:
	Sprite(std::string_view file);
	~Sprite() override;

	[[nodiscard]] std::string_view type() const override;
	void toggle_antialiasing(bool val) const;
	bool set_texture(std::string_view file);
	void set_texture_wrap(texture_wrap type);
	void set_texture_source_rect(rect<float> val);
	[[nodiscard]] rect<float> texture_source_rect() const;

protected:
	bool init(Context const& ctx) override;
	void draw(Context const& ctx, mat3 const& transform, rgb color, float alpha) const override;

	std::weak_ptr<Resource> resource_wref_;
	std::shared_ptr<Texture> texture_;
	rect<float> texture_source_rect_;
	std::string file_arg_;
};

}

#endif