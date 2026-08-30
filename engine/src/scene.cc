#include <aether/node/node.hh>
#include <aether/scene.hh>

namespace aether {

bool scene::add_child(strong_ref<node> n) { return root_node_->add_child(n); }

bool scene::init_() {
	root_node_         = node::create<node>(ctx_);
	root_node_->scene_ = this;
	return true;
}

void scene::update_all_(float dt) {
	root_node_->update_(dt);
	update_(dt);
}

void scene::draw_all_() {
	root_node_->draw_();
	visit_();
}

} // namespace aether