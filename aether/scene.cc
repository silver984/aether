#include <cassert>
#include <context.hh>
#include <debug/log.hh>
#include <nodes/node.hh>
#include <scene.hh>
#include <sound.hh>
#include <window.hh>

namespace aether {

scene::scene(context const& ctx)
        : mctx_(ctx)
        , camera_(root_node_)
        , root_node_(node::create<node>(ctx))
        , is_active_(false)
        , is_visit_scheduled_(false) {
	assert(root_node_ != nullptr);
	root_node_->mscene_         = this;
	size<int> const window_size = ctx.core().fetch_window().target_size();
	root_node_->set_bounds(static_cast<size<int>>(window_size));
	root_node_->set_position(window_size / 2.f);
}

scene::~scene() = default;

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

bool scene::add(std::shared_ptr<node> _node) {
	return root_node_->add_child(_node);
}

bool scene::add(std::shared_ptr<sound> _sound) {
	if (!_sound) {
		return false;
	}

	sounds_.emplace_back(_sound);
	return true;
}

std::shared_ptr<node> scene::root_node() const {
	return root_node_;
}

camera& scene::get_camera() {
	return camera_;
}

bool scene::init_() {
	return true;
}

void scene::update_(float dt) {}

void scene::visit_() {}

context const& scene::ctx_() const {
	return mctx_;
}

bool scene::init_scene_() {
	return init_();
}

void scene::update_all_(float dt) {
	// auto release sounds
	std::erase_if(sounds_, [](auto const& sound) {
		return sound.use_count() <= 1 && sound->voice_count_() == 0;
	});

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