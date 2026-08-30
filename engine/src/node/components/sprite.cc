#include <aether/context.hh>
#include <aether/node/components/sprite.hh>
#include <aether/node/components/transform.hh>
#include <aether/node/node.hh>
#include <aether/resources.hh>

namespace aether {

sprite::sprite(context const& ctx, strong_ref<node> n, sprite_args const& args) noexcept
        : node_component(ctx, n)
        , maybe_args_(unique_ref<sprite_args>::create(args)) {
}
sprite::~sprite() noexcept = default;

bool sprite::init_() noexcept {
	if (!node_component::init_()) {
		maybe_args_.release();
		return false;
	}

	strong_ref<node> strong_node = this->weak_node_.construct();
	if (strong_node && !strong_node->component<transform>()) {
		if (!strong_node->add_component<transform>()) {
			return false;
		}
	}

	texture = this->ctx_.textures->load(maybe_args_->pak, maybe_args_->file);

	if (!texture) {
		maybe_args_.release();
		return false;
	}

	maybe_args_.release();
	return true;
}

} // namespace aether