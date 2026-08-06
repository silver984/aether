#include <aether/context.hh>
#include <aether/log.hh>
#include <aether/node.hh>
#include <aether/scene.hh>
#include <aether/sound.hh>
#include <aether/window.hh>
#include <cassert>

namespace aether {

scene::scene(context const& ctx) noexcept
        : m_ctx_(ctx)
        // , camera_(root_node_)
        , root_node_(nullptr)
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

bool scene::add(strong_ref<node> n) {
	return root_node_->add_child(n);
}

bool scene::add(strong_ref<sound> s) {
	if (!s) {
		return false;
	}
	sounds_.emplace_back(s);
	return true;
}

weak_ref<node> scene::root_node() const {
	return root_node_;
}

// camera& scene::get_camera() {
// 	return camera_;
// }

bool scene::init_() {
	root_node_ = node::create<node>(m_ctx_);

	if (!root_node_) {
		return false;
	}

	root_node_->scene_ = this;

	size<int> const window_size = m_ctx_.window().target_size();
	root_node_->set_bounds(window_size);
	root_node_->set_position(window_size / 2.f);

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
	if (!root_node_) {
		return;
	}
	root_node_->update_all_(dt);
	if (is_active_) {
		update_(dt);
	}
}

void scene::draw_all_() {
	if (!root_node_) {
		return;
	}
	root_node_->draw_all_();
	if (is_visit_scheduled_) {
		visit_();
	}
}

} // namespace aether