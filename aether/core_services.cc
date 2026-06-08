#include <core_services.hh>

namespace aether {

// private
core_services::core_services(window& window, renderer& renderer, SoLoud::Soloud& soloud)
        : window_(window)
        , renderer_(renderer)
        , soloud_(soloud) {}

core_services::~core_services() = default;

window& core_services::fetch_window() const {
	return window_;
}

renderer& core_services::fetch_renderer() const {
	return renderer_;
}

SoLoud::Soloud& core_services::soloud() const {
	return soloud_;
}

} // namespace aether