#pragma once
#include <array>
#include <objects/Node.hh>

namespace aether {

class AnimatedSprite;

class PopupUI : public Node {
	enum area {
		top_left_corner,
		top,
		top_right_corner,
		right,
		bottom_right_corner,
		bottom,
		bottom_left_corner,
		left,
		count
	};

public:
	PopupUI(Context const& ctx);
	~PopupUI() override;

protected:
	bool init() override;

private:
	void set_area_animation(AnimatedSprite& area_sprite, int index);

	std::array<std::shared_ptr<AnimatedSprite>, area::count> areas_;
};

} // namespace aether