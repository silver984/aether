#ifndef __AETHER_FUNKIN_HH__
#define __AETHER_FUNKIN_HH__

#include <aether/common/Context.hh>
#include <aether/math/size.hh>
#include <aether/systems/Director.hh>
#include <aether/systems/Renderer.hh>
#include <aether/systems/Resource.hh>
#include <aether/systems/Window.hh>
#include <cstdint>
#include <memory>
#include <string_view>

namespace ae {

class Funkin final {
public:
	Funkin();
	~Funkin();
	Funkin(Funkin const&)            = delete;
	Funkin(Funkin&&)                 = delete;
	Funkin& operator=(Funkin const&) = delete;
	Funkin& operator=(Funkin&&)      = delete;

	bool init(std::string_view game_title, size<int> game_resolution, int game_fps);
	void run();
	[[nodiscard]] Context const& context();

private:
	void shutdown();

	std::shared_ptr<Window> window_;
	std::shared_ptr<Renderer> renderer_;
	std::shared_ptr<Resource> resource_;
	std::shared_ptr<Director> director_;
	Context ctx_;
	bool is_initialized_;
};

} // namespace ae

#endif