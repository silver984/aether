#pragma once
#include <aether/objects/Node.hh>
#include <aether/graphics/Texture.hh>
#include <aether/common/pointers.hh>
#include <string_view>
#include <string>

namespace ae {

class Graphic : public Node {
public:
	Graphic(std::string_view file);
	~Graphic() override;

	[[nodiscard]] std::string_view type() const override;

protected:
	bool init(Context const& ctx) override;
	void draw(Context const& ctx, mat3 const& transform, float alpha) const override;

private:
	std::string file_arg_;
	sptr<Texture> texture_;
};

}