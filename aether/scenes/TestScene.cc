#include <Context.hh>
#include <cmath>
#ifdef AETHER_DEBUG
	#include <log.hh>
#endif
#include <objects/AnimatedSprite.hh>
#include <objects/TileMap.hh>
#include <objects/abstract/Sound.hh>
#include <scenes/TestScene.hh>
#include <services/core/Renderer.hh>
#include <services/core/Window.hh>

using namespace aether;

TestScene::TestScene(Context const& ctx)
    : Scene(ctx)
    , elapsed_(0.f) {}

TestScene::~TestScene() = default;

// protected
bool TestScene::init() {
	queue_work([this]() -> void {
		if (auto animation =
		        Node::create<AnimatedSprite>(ctx_, AnimatedSprite::descriptor{.image_file = "resources/spooky_dark.png",
		                                                                      .data_file  = "resources/spooky_dark.xml",
		                                                                      .fps        = 24})) {
			animation->play_animation("spooky dance idle", {.loop = true});
			animation->set_position(vec2<float>(600.f, 400.f));
			add_child(animation);
		}
	});

	queue_work([this]() -> void {
		if (auto icon = Node::create<TileMap>(
		        ctx_, TileMap::descriptor{.file = "resources/icon-bf-old.png", .tile_bounds = size<int>(150)})) {
			icon->set_position(vec2<float>(800.f, 400.f));
			icon->set_rotation(45.f);
			icon->set_name("icon");
			add_child(icon);
		}
	});

	queue_work([this]() -> void {
		if (auto animation = Node::create<AnimatedSprite>(
		        ctx_, AnimatedSprite::descriptor{.image_file = "resources/cuttinDeezeBalls.png",
		                                         .data_file  = "resources/cuttinDeezeBalls.xml",
		                                         .fps        = 24})) {
			animation->play_animation("Whitty Ballistic Cutscene", {.loop = true});
			animation->set_scale(0.8f);
			animation->set_position(vec2<float>(400.f, 400.f));
			add_child(animation);
		}
	});

	queue_work([this]() -> void {
		if (sound_ = Sound::create(ctx_, "resources/Inst.mp3")) {
			(void)sound_->play();
		}
	});

	activate();
	return true;
}

// protected
void TestScene::enter() {
	ctx_.core_services.renderer.set_background_rgba(rgba(128, 128, 128, 255));
}

// protected
void TestScene::update(float dt) {
	if (!sound_ || sound_->is_finished()) {
		return;
	}

	elapsed_ += dt;
	while (elapsed_ >= 1.f) {
		(void)sound_->seek_time(sound_->time() + 5.f);
		elapsed_ -= 1.f;
	}

#ifdef AETHER_VERBOSE_DEBUG
	tracelog("Sound time: {:.2f}", sound_->time());
#endif
}