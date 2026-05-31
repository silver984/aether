#ifdef AETHER_DEBUG
	#include <aether/debug/log.hh>
#endif
#include <aether/systems/Context.hh>
#include <aether/systems/Director.hh>
#include <aether/util/timer.hh>
#include <utility>

namespace ae {

// private
Director::Director()  = default;
Director::~Director() = default;

void Director::switch_state(std::shared_ptr<Node>&& new_state) {
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
void Director::try_cleanup() {
	if (pending_state_) {
		current_state_.reset();
	}

	if (current_state_) {
		pending_state_.reset();
	}
}

// private
void Director::update_current_state(float dt) {
	if (pending_state_) {
		if (current_state_) {
			current_state_.reset();
		}

		current_state_ = std::move(pending_state_);
	}

	if (current_state_) {
		current_state_->base_update(dt);
	}
}

// private
void Director::draw_current_state() {
	if (current_state_) {
		current_state_->base_draw();
	}
}

} // namespace ae