#include <aether/systems/Director.hh>
#include <aether/systems/Window.hh>
#include <aether/common/log.hh>
#include <fmt/format.h>
#include <utility>

namespace ae {

// private
Director::Director() = default;

// private
Director::~Director() = default;

void Director::switch_state(std::shared_ptr<Node>&& new_state) {
	if (!new_state) {
		log::error("Attempted to switch to a nullptr state");
		return;
	}

	pending_state_ = std::move(new_state);

	log::debug("Switching states");
}

// private
void Director::update_current_state(Context const& ctx) {
	if (pending_state_) {
		move_pending_state();
	}

	if (!current_state_) {
		return;
	}

	if (
		auto window = ctx.window().lock();
		window->was_resized()
	) {
		current_state_->mark_dirty();
	}

	current_state_->base_update(ctx, ctx.delta_time());
}

// private
void Director::draw_current_state(Context const& ctx) {
	if (current_state_) {
		current_state_->base_draw(ctx);
	}
}

// private
void Director::move_pending_state() {
	if (current_state_) {
		log::trace(fmt::format("Releasing current state ({})", fmt::ptr(current_state_.get())));

		current_state_.reset();
		current_state_ = nullptr;

		log::trace("Current state released");
	}

	log::trace(fmt::format("Moving pending state ({})", fmt::ptr(pending_state_.get())));

	if (current_state_ = std::move(pending_state_)) {
		log::trace("Moved pending state to current");

		pending_state_ = nullptr;
		current_state_->mark_dirty();

		log::info("Switched state");
	}
}

}