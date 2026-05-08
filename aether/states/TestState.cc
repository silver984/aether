#include <aether/states/TestState.hh>
#include <aether/states/TestState2.hh>
#include <aether/systems/Director.hh>
#include <aether/systems/Renderer.hh>
#include <cmath>

TestState::TestState() :
	elapsed_(0.f)
{};

TestState::~TestState() = default;

bool TestState::init(ae::Context const& ctx) {
	if (grah_ = ae::Node::create<ae::Graphic>(ctx, "resources/grah.png")) {
		add(grah_);
		activate();
		return true;
	}

	return false;
}

void TestState::update(ae::Context const& ctx, float dt) {
	elapsed_ += dt;

	float speed = 2.0f;
	float oscillating = 0.5f * (std::sin(speed * elapsed_) + 1.f);

	if (auto renderer = ctx.renderer().lock()) {
		renderer->set_background_rgba(renderer->background_rgba().first, oscillating);
		grah_->set_scale(ae::vec2<float>(oscillating, oscillating));
	}

	grah_->set_position(grah_->position() + (10.f * dt));
	grah_->set_rotation(grah_->rotation() - (90.f * dt));
}
