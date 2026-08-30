#include <aether/node/components/component.hh>

namespace aether {

node_component::node_component(context const& ctx, strong_ref<node> n)
        : ctx_(ctx)
        , weak_node_(n)
        , time_scale_(1.f)
        , scheduled_(schedule_level::none) {
}
node_component::~node_component() = default;

void node_component::schedule(schedule_level level) noexcept {
	scheduled_ |= level;
}

void node_component::unschedule(schedule_level level) noexcept {
	scheduled_ &= ~level;
}

bool node_component::is_scheduled(schedule_level level) const noexcept {
	return (scheduled_ & level) != schedule_level::none;
}

bool node_component::init_() noexcept {
	return true;
}

void node_component::update_(float dt) noexcept {
}

void node_component::visit_() noexcept {
}

void node_component::draw_() noexcept {
}

bool node_component::init_interface_() noexcept {
	return init_();
}

} // namespace aether