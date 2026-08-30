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
	boy->set_name("boy");
	{
		sprite* s = boy->add_component<sprite>();
		s->set_texture(zip_archive("aether.pak"), "boy");
		s->set_antialiasing(true);

		transform* t = boy->component<transform>();
		t->set_scale(0.6f);
		t->set_position(this->ctx_.window->target_size() * 0.5f);
		this->add_child(boy);
	}

	strong_ref<node> silly = node::create(this->ctx_);
	silly->set_name("silly");
	{
		sprite* s = silly->add_component<sprite>();
		s->set_texture(zip_archive("aether.pak"), "cats.silly");
		s->set_antialiasing(true);

		transform* silly_t = silly->component<transform>();
		transform* boy_t   = boy->component<transform>();
		silly_t->set_scale(0.2f);
		silly_t->set_position(boy_t->position() - 120.f);
		boy->add_child(silly);
	}

	return true;
}

void testscene::update_(float dt) {
	scene::update_(dt);
	constexpr float ROTATION_VAL = 22.5f;
	for (auto& child : root_node().construct()->children()) {
		if (child->name() != "boy") {
			return;
		}

		for (auto& boy_ch : child->children()) {
			transform* t = boy_ch->component<transform>();
			if (!t) {
				return;
			}
			t->set_rotation(t->rotation() + ((ROTATION_VAL * 2) * dt));
		}

		{
			transform* t = child->component<transform>();
			if (!t) {
				return;
			}
			t->set_rotation(t->rotation() + (ROTATION_VAL * dt));
			if (t->rotation() >= 90.f) {
				child->remove_component<transform>();
			}
		}
	}
}