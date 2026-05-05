#include <aether/systems/Director.hh>
#include <aether/systems/Window.hh>
#include <utility>

namespace ae {

// private
Director::Director() = default;

// private
Director::~Director() = default;

void Director::switch_state(std::shared_ptr<Node>&& new_state) {
	if (!new_state) {
		// TODO: log error
		return;
	}

	pending_state_ = std::move(new_state);
}

// private
void Director::update_current_state(Context const& ctx) {
	if (pending_state_) {
		current_state_.reset();
		current_state_ = nullptr;

		if (current_state_ = std::move(pending_state_)) {
			pending_state_ = nullptr;
			current_state_->mark_dirty();
		}
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

}