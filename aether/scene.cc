#include <cassert>
#include <context.hh>
#include <debug/log.hh>
#include <nodes/node.hh>
#include <scene.hh>
#include <sound.hh>
#include <window.hh>

namespace aether {

scene::scene(context const& ctx) noexcept
        : m_ctx_(ctx)
        // , camera_(root_)
        , root_(nullptr)
        , is_active_(false)
        , is_visit_scheduled_(false) {
}
scene::~scene() noexcept = default;

void scene::activate() {
	is_active_ = true;
}

void scene::deactivate() {
	is_active_ = false;
}

void scene::schedule_visit() {
	is_visit_scheduled_ = true;
}

void scene::unschedule_visit() {
	is_visit_scheduled_ = false;
}

bool scene::add(ref<node> n) {
	return root_->add_child(n);
}

bool scene::add(ref<sound> s) {
	if (!s) {
		return false;
	}
	sounds_.emplace_back(s);
	return true;
}

ref<node> scene::root() const {
	return root_;
}

// camera& scene::get_camera() {
// 	return camera_;
// }

bool scene::init_() {
	root_ = new node(m_ctx_);

	if (!root_) {
		return false;
	}

	root_->scene_ = this;

	size<int> const window_size = m_ctx_.window().target_size();
	root_->set_bounds(window_size);
	root_->set_position(window_size / 2.f);

	return true;
}

void scene::update_(float dt) {
}

void scene::visit_() {
}

context const& scene::ctx_() const {
	return m_ctx_;
}

bool scene::init_interface_() {
	return init_();
}

void scene::update_all_(float dt) {
	root_->update_all_(dt);
	if (is_active_) {
		update_(dt);
	}
}

void scene::draw_all_() {
	root_->draw_all_();
	if (is_visit_scheduled_) {
		visit_();
	}
}

} // namespace aether