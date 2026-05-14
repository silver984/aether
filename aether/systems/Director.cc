#include <aether/systems/Director.hh>
#include <aether/systems/Resource.hh>
#include <aether/common/log.hh>
#include <aether/common/timer.hh>
#include <utility>

namespace ae {

// private
Director::Director(Context const& ctx) :
	context_(&ctx)
{}

// private
Director::~Director() = default;

void Director::switch_state(std::shared_ptr<Node>&& new_state) {
	if (!new_state) {
		debuglog("Attempted to switch to a nullptr state");
		
		if (auto resource = context_->resource_wref().lock()) {
			resource->try_clean_refs();
		}

		return;
	}

	pending_state_ = std::move(new_state);

	tracelog("Switching states | pending: {}", fmt::ptr(pending_state_.get()));
}

void Director::try_cleanup() {
	debuglog("Attempting to clean up");

	auto start_time = timer::start();

	if (pending_state_) {
		release_pending_state();
	}

	if (current_state_) {
		release_current_state();
	}

	auto end_time = timer::end(start_time);

	debuglog("Done | took {}ms", end_time);
}

// private
void Director::release_current_state() {
	current_state_.reset();
	tracelog("Current state released ({})", fmt::ptr(current_state_.get()));
}

// private
void Director::release_pending_state() {
	pending_state_.reset();
	tracelog("Pending state released ({})", fmt::ptr(pending_state_.get()));
}

// private
void Director::update_current_state() {
	if (pending_state_) {
		move_pending_state();
	}

	if (current_state_) {
		current_state_->base_update(context_->delta_time());
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

		if (auto resource = context_->resource_wref().lock()) {
			resource->try_clean_refs();
		}
	}

	current_state_ = std::move(pending_state_);
	tracelog("Moved pending state to current");
}

}