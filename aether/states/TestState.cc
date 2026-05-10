#include <aether/states/TestState.hh>
#include <aether/objects/Sprite.hh>
// #include <aether/objects/TileMap.hh>
#include <aether/systems/Window.hh>
#include <aether/systems/Renderer.hh>
// #include <aether/common/log.hh>
#include <cmath>

TestState::TestState() :
	elapsed_(0.f),
	long_height_(0.f)
{}

TestState::~TestState() = default;

// protected
bool TestState::init(ae::Context const& ctx) {
	if (auto renderer = ctx.renderer_wref().lock()) {
		renderer->set_background_rgba(ae::rgb(ae::rgb::as_float(202), ae::rgb::as_float(255), ae::rgb::as_float(77)), 1.f);
	}

	if (long_ = ae::Node::create<ae::Sprite>(ctx, "resources/long.png")) {
		if (auto window = ctx.window_wref().lock()) {
			long_->set_position(static_cast<ae::vec2<float>>(window->screen_size()) / 2.f);
		}

		add(long_);
		long_height_ = long_->texture_source_rect().height;

		if (long_tail_ = ae::Node::create<ae::Sprite>(ctx, "resources/long-tail.png")) {
			long_tail_->set_anchor(ae::vec2<float>(0.5f, 0.f));
			long_->add(long_tail_);
			update_long_trail();
		}

		activate();

		return true;
	}

	return false;
}

// protected
void TestState::update(ae::Context const& ctx, float dt) {
	elapsed_ += dt;

	while (elapsed_ >= 1.f) {
		if (long_) {
			auto v = long_->texture_source_rect();
			v.height += long_height_;
			long_->set_texture_source_rect(v, true);
		}

		update_long_trail();

		elapsed_ -= 1.f;
	}
}

// private
void TestState::update_long_trail() {
	if (!long_tail_) {
		return;
	}

	auto p = long_tail_->parent().lock();

	if (!p) {
		return;
	}

	ae::vec2<float> v = {
		.x = p->bounds().width / 2.f,
		.y = p->bounds().height
	};

	long_tail_->set_position(v);
}