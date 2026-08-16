#include <aether/sprite.hh>
#include <aether/testscene.hh>
#include <aether/window.hh>
#include <aether/zip_archive.hh>

using namespace aether;

testscene::testscene() noexcept  = default;
testscene::~testscene() noexcept = default;

bool testscene::init_() {
	if (!scene::init_()) {
		return false;
	}

	size<int> const window_size = window::instance()->target_size();
	zip_archive pak             = "aether.pak";

	auto boy = node::create<sprite>(sprite_args{
	        .pak  = pak,
	        .file = "boy",
	});
	boy->set_scale(0.6f);
	boy->set_position(window_size * 0.5f);
	this->add(boy);

	auto silly = node::create<sprite>(sprite_args{
	        .pak  = pak,
	        .file = "cats.silly",
	});
	silly->set_scale(0.2f);
	silly->set_position(boy->position() - 200.f);
	this->add(silly);

	auto traffic_cones = node::create<sprite>(sprite_args{
	        .pak  = pak,
	        .file = "cats.funny.traffic-cones",
	});
	traffic_cones->set_scale(0.4f);
	traffic_cones->set_position(boy->position() + 200.f);
	this->add(traffic_cones);

	this->activate();

	return true;
}

void testscene::update_(float dt) {
	scene::update_(dt);

	if (auto root = root_node().construct()) {
		for (auto& child : root->children()) {
			child->set_rotation(child->rotation() + (22.5f * dt));
		}
	}
}