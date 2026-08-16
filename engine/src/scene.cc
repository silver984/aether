#include <aether/context.hh>
#include <aether/log.hh>
#include <aether/node.hh>
#include <aether/scene.hh>
#include <aether/window.hh>
#include <cassert>

namespace aether {

scene::scene(context const& ctx) noexcept
        : ctx(ctx)
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

weak_ref<node> scene::root_node() const {
	return root_node_;
}

bool scene::init_() {
	root_node_ = node::create<node>(ctx);

	if (!root_node_) {
		return false;
	}

	root_node_->scene_ = this;

	size<int> const window_size = ctx.window.target_size();
	root_node_->set_bounds(window_size);
	root_node_->set_position(window_size / 2.f);

	return true;
}

void scene::update_(float dt) {
}

void scene::visit_() {
}

bool scene::init_interface_() {
	return init_();
}

void scene::update_all_(float dt) {
	root_node_->update_all_(dt);
	if (is_active_) {
		update_(dt);
	}
}

void scene::draw_all_() {
	root_node_->draw_all_();
	if (is_visit_scheduled_) {
		visit_();
	}
}

} // namespace aether