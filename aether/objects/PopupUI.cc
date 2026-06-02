#include <Context.hh>
#include <objects/AnimatedSprite.hh>
#include <objects/PopupUI.hh>
#include <services/Window.hh>

namespace aether {

PopupUI::PopupUI(Context const& ctx)
    : NodeIdentity<PopupUI>(ctx) {}

PopupUI::~PopupUI() = default;

// protected
bool PopupUI::init() {
	// for (int i = 0; i < count; ++i) {
	// 	auto& area = areas_[i];

	// 	area = Node::create<AnimatedSprite>(ctx_, "resources/aether/ui/blackbox.png",
	// 	                                    "resources/aether/ui/blackbox.xml", 2);

	// 	if (!area) {
	// 		return false;
	// 	}

	// 	set_area_animation(*area, i);
	// 	add_child(area);
	// }

	// areas_[top]->set_position_x(static_cast<float>(areas_[top_left_corner]->width()));
	// areas_[top_right_corner]->set_position_x(areas_[top]->position().x + areas_[top]->width());

	// areas_[right]->set_scale_y((areas_[right]->width() + 1) / static_cast<float>(areas_[right]->width()));
	// areas_[right]->toggle_flip(true);
	// areas_[right]->set_position_x(areas_[top_right_corner]->position().x);
	// areas_[right]->set_position_y(static_cast<float>(areas_[top_right_corner]->height()));

	// areas_[bottom_right_corner]->set_position_x(areas_[right]->position().x);
	// areas_[bottom_right_corner]->set_position_y(areas_[right]->position().y + areas_[top]->height());
	// areas_[bottom]->set_position_x(areas_[top]->position().x);
	// areas_[bottom]->set_position_y(areas_[bottom_right_corner]->position().y);
	// areas_[bottom_left_corner]->set_position_y(areas_[bottom_right_corner]->position().y);

	// areas_[left]->set_position_y(areas_[right]->position().y);

	// set_position(ctx_.window.screen_size() / 2.f);

	return true;
}

// private
void PopupUI::set_area_animation(AnimatedSprite& area_sprite, int index) {
	// switch (index) {
	// case top_left_corner: {
	// 	area_sprite.play_anim("top_left_corner", true);
	// 	break;
	// }
	// case top: {
	// 	area_sprite.play_anim("top", true);
	// 	break;
	// }
	// case top_right_corner: {
	// 	area_sprite.play_anim("top_right_corner", true);
	// 	break;
	// }
	// case right: {
	// 	area_sprite.play_anim("left", true);
	// 	break;
	// }
	// case bottom_right_corner: {
	// 	area_sprite.play_anim("bottom_right_corner", true);
	// 	break;
	// }
	// case bottom: {
	// 	area_sprite.play_anim("bottom", true);
	// 	break;
	// }
	// case bottom_left_corner: {
	// 	area_sprite.play_anim("bottom_left_corner", true);
	// 	break;
	// }
	// case left: {
	// 	area_sprite.play_anim("left", true);
	// 	break;
	// }
	// default: break;
	// }
}

} // namespace aether
