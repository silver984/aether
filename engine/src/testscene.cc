#include <context.hh>
#include <debug/log.hh>
// #include <lua/manager.hh>
// #include <lua/templates.hh>
#include <nodes/sprite.hh>
#include <testscene.hh>
#include <window.hh>

using namespace aether;

testscene::testscene(context const& ctx) noexcept
        : scene(ctx) {
}
testscene::~testscene() noexcept = default;

bool testscene::init_() {
	if (!scene::init_()) {
		return false;
	}

	auto const& ctx  = ctx_();
	auto window_size = ctx.window().target_size();
	auto spr         = node::create<sprite>(ctx, sprite_args{.file = "boy.png"});
	if (!spr) {
		return false;
	}

	spr->set_scale(0.6f);
	spr->set_position(window_size / 2.f);
	add(spr);
	activate();

	return true;
}

void testscene::update_(float dt) {
	if (auto root = root_node().construct()) {
		for (auto& child : root->children()) {
			child->set_rotation(child->rotation() + (22.5f * dt));
		}
	}
}