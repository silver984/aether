#ifndef __AETHER_OBJECTS_GRAPHIC_HH__
#define __AETHER_OBJECTS_GRAPHIC_HH__

#include <aether/objects/Node.hh>
#include <memory>
#include <string_view>
#include <string>
#include <optional>

struct Texture;

namespace ae {

class Graphic : public Node {
public:
	Graphic(std::string_view file);
	~Graphic() override;

	[[nodiscard]] std::string_view type() const override;
	void toggle_antialiasing(bool val) const;

protected:
	bool init(Context const& ctx) override;
	void draw(Context const& ctx, mat3 const& transform, float alpha) const override;

private:
	std::string file_arg_;
	std::shared_ptr<Texture> texture_;
};

}

#endif