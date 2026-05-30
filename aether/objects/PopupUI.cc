#include <aether/objects/AnimatedSprite.hh>
#include <aether/objects/PopupUI.hh>
#include <aether/systems/Window.hh>

namespace ae {

PopupUI::PopupUI(Context const& ctx) : NodeIdentity<PopupUI>(ctx) {}
PopupUI::~PopupUI() = default;

// protected
bool PopupUI::init() {
	for (int i = 0; i < area::count; ++i) {
		auto& area = areas_[i];

		area = Node::create<AnimatedSprite>(ctx_, "resources/aether/ui/blackbox.png",
		                                    "resources/aether/ui/blackbox.xml", 3);

		if (!area) {
			return false;
		}

		set_area_animation(*area, i);
		add(area);
	}

	areas_[area::top]->set_position({static_cast<float>(areas_[area::top_left_corner]->bounds().width), 0.f});

	areas_[area::top_right_corner]->set_position(
	    {areas_[area::top]->position().x + static_cast<float>(areas_[area::top]->bounds().width), 0.f});

	set_position(ctx_.window.screen_size() / 2.f);

	return true;
}

// private
void PopupUI::set_area_animation(AnimatedSprite& area_sprite, int index) {
	switch (index) {
	case area::top_left_corner: {
		area_sprite.play_anim("top_left_corner", true);
		break;
	}
	case area::top: {
		area_sprite.play_anim("top", true);
		break;
	}
	case area::top_right_corner: {
		area_sprite.play_anim("top_left_corner", true);
		break;
	}
	case area::right: {
		area_sprite.play_anim("left", true);
		break;
	}
	case area::bottom_right_corner: {
		area_sprite.play_anim("bottom_left_corner", true);
		break;
	}
	case area::bottom: {
		area_sprite.play_anim("bottom", true);
		break;
	}
	case area::bottom_left_corner: {
		area_sprite.play_anim("bottom_left_corner", true);
		break;
	}
	case area::left: {
		area_sprite.play_anim("left", true);
		break;
	}
	default: break;
	}
}

} // namespace ae
