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

	zip_archive pak("aether.pak");
	strong_ref<node> boy = node::create(this->ctx_);
	if (!boy) {
		return false;
	}

	{
		sprite* s = boy->add_component<sprite>();
		s->set_texture(pak, "boy");
		s->set_antialiasing(true);

		transform* t = boy->component<transform>();
		t->set_scale(0.6f);
		t->set_position(window::bounds() * 0.5f);
		this->add_child(boy);
	}

	strong_ref<node> silly = node::create(this->ctx_);
	if (!silly) {
		return false;
	}

	{
		sprite* s = silly->add_component<sprite>();
		s->set_texture(pak, "cats.silly");
		s->set_antialiasing(true);

		transform* silly_t = silly->component<transform>();
		transform* boy_t   = boy->component<transform>();
		silly_t->set_scale(0.2f);
		silly_t->set_position(boy_t->position() - 120.f);
		boy->add_child(silly);
	}

	return true;
}