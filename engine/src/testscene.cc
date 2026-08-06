#include <context.hh>
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

	context const& ctx          = this->ctx_();
	size<int> const window_size = ctx.window().target_size();

	auto boy = node::create<sprite>(ctx, sprite_args{
	                                             .file = "boy",
	                                     });
	if (!boy) {
		return false;
	}
	boy->set_scale(0.6f);
	boy->set_position(window_size * 0.5f);
	this->add(boy);

	auto silly = node::create<sprite>(ctx, sprite_args{
	                                               .file = "cats.silly",
	                                       });
	if (!silly) {
		return false;
	}
	silly->set_scale(0.2f);
	silly->set_position(boy->position() - 200.f);
	this->add(silly);

	auto traffic_cones = node::create<sprite>(ctx, sprite_args{
	                                                       .file = "cats.funny.traffic-cones",
	                                               });
	if (!traffic_cones) {
		return false;
	}
	traffic_cones->set_scale(0.4f);
	traffic_cones->set_position(boy->position() + 200.f);
	this->add(traffic_cones);

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