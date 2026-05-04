#include <aether/systems/Director.hh>
#include <utility>

namespace ae {

// private
Director::Director() :
	current_state_(nullptr),
	pending_state_(nullptr)
{}

// private
Director::~Director() = default;

void Director::switch_state(sptr<Node>&& new_state) {
	if (!new_state) {
		// TODO: log error
		return;
	}

	pending_state_ = std::move(new_state);
}

// private
void Director::update_current_state(Context const& ctx) {
	if (pending_state_) {
		current_state_ = std::move(pending_state_);
	}

	if (current_state_) {
		current_state_->base_update(ctx, ctx.delta_time());
	}
}

// private
void Director::draw_current_state(Context const& ctx) {
	if (current_state_) {
		current_state_->base_draw(ctx);
	}
}

}