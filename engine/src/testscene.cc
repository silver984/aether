#include <aether/context.hh>
#include <aether/node/components/sprite.hh>
#include <aether/node/components/transform.hh>
#include <aether/node/node.hh>
#include <aether/testscene.hh>
#include <aether/window.hh>
#include <aether/zip_archive.hh>

using namespace aether;

bool testscene::init_() {
	if (!scene::init_()) {
		return false;
	}

	strong_ref<node> boy = node::create(this->ctx_);
	sprite* s            = boy->add_component<sprite>();
	s->set_texture(zip_archive("aether.pak"), "boy");
	s->set_antialiasing(true);

	transform* t = boy->component<transform>();
	t->set_scale(0.6f);
	t->set_position(this->ctx_.window->target_size() * 0.5f);
	this->add_child(boy);

	return true;
}

void testscene::update_(float dt) {
	scene::update_(dt);
	for (auto& child : root_node().construct()->children()) {
		if (transform* t = child->component<transform>()) {
			t->set_rotation(t->rotation() + (22.5f * dt));
			if (t->rotation() >= 90.f) {
				child->remove_component<transform>();
			}
		}
	}
}