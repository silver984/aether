#ifndef __AETHER_OBJECTS_SPRITE_HH__
#define __AETHER_OBJECTS_SPRITE_HH__

#include <aether/objects/Node.hh>
#include <memory>
#include <string_view>
#include <string>

struct Texture;

namespace ae {

class Sprite : public Node {
public:
	Sprite(std::string_view file);
	~Sprite() override;

	[[nodiscard]] std::string_view type() const override;
	void toggle_antialiasing(bool val) const;

protected:
	bool init(Context const& ctx) override;
	void draw(Context const& ctx, mat3 const& transform, rgb color, float alpha) const override;

	std::shared_ptr<Texture> texture_;
	std::string file_arg_;
};

}

#endif