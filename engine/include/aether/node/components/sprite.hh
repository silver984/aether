#pragma once
#include <aether/node/components/component.hh>
#include <string_view>

struct Texture;

namespace aether {

class zip_archive;

struct sprite_args final {
	zip_archive const& pak;
	std::string_view file;
};

// this component adds the transform component if the node doesnt have it yet
class sprite final : public node_component {
public:
	sprite(context const& ctx, strong_ref<node> n, sprite_args const& args) noexcept;
	~sprite() noexcept override;

	strong_ref<Texture> texture;

protected:
	bool init_() noexcept override;

private:
	unique_ref<sprite_args> maybe_args_;
};

} // namespace aether