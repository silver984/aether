#ifdef AETHER_DEBUG
	#include <log.hh>
#endif
#include <Context.hh>
#include <services/SceneScheduler.hh>
#include <util/timer.hh>
#include <utility>

namespace aether {

// private
SceneScheduler::SceneScheduler()  = default;
SceneScheduler::~SceneScheduler() = default;

void SceneScheduler::switch_state(std::shared_ptr<Node>&& new_state) {
	if (!new_state) {
#ifdef AETHER_DEBUG
		errorlog("Can't switch to nullptr state");
#endif
		return;
	}

	pending_state_ = std::move(new_state);

#ifdef AETHER_VERBOSE_DEBUG
	tracelog("Switching states | pending: {}", fmt::ptr(pending_state_.get()));
#endif
}

// private
void SceneScheduler::cleanup() {
	current_state_.reset();
	pending_state_.reset();
}

// private
void SceneScheduler::update_current_state(float dt) {
	if (pending_state_) {
		current_state_ = std::move(pending_state_);
	}

	if (current_state_) {
		current_state_->base_update(dt);
	}
}

// private
void SceneScheduler::draw_current_state() {
	if (current_state_) {
		current_state_->base_draw();
	}
}

// private
bool SceneScheduler::has_pending_state() const {
	return pending_state_ != nullptr;
}

} // namespace aether