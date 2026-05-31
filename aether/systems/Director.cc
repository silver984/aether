#include <aether/debug/log.hh>
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
		debuglog("Attempted to switch to a nullptr state");
		return;
	}

	pending_state_ = std::move(new_state);
	tracelog("Switching states | pending: {}", fmt::ptr(pending_state_.get()));
}

// private
void Director::try_cleanup() {
	debuglog("Attempting to clean up");
	auto const start_time = util::timer::start();

	if (pending_state_) {
		release_pending_state();
	}

	if (current_state_) {
		release_current_state();
	}

	auto const end_time = util::timer::end(start_time);
	debuglog("Done | took {}ms", end_time);
}

// private
void Director::release_current_state() {
	current_state_.reset();
	tracelog("Current state released");
}

// private
void Director::release_pending_state() {
	pending_state_.reset();
	tracelog("Pending state released");
}

// private
void Director::update_current_state(float dt) {
	if (pending_state_) {
		move_pending_state();
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

// private
void Director::move_pending_state() {
	if (current_state_) {
		release_current_state();
	}

	current_state_ = std::move(pending_state_);
	tracelog("Moved pending state to current");
}

} // namespace ae