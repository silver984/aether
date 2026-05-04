#pragma once
#include <aether/math/size.hh>
#include <aether/systems/Window.hh>
#include <aether/systems/Renderer.hh>
#include <aether/systems/Resource.hh>
#include <aether/systems/Director.hh>
#include <aether/common/Context.hh>
#include <string_view>

namespace ae {

class Funkin final {
public:
	Funkin();
	~Funkin();
	Funkin(Funkin const&) = delete;
	Funkin(Funkin&&) = delete;
	Funkin& operator =(Funkin const&) = delete;
	Funkin& operator =(Funkin&&) = delete;

	bool init(std::string_view game_title, size<int> const& game_resolution, int game_fps);
	void run();
	[[nodiscard]] Context context();
	
private:
	void shutdown();
	void update_dpi_scale(Context& ctx);
	void update_frame_ctx(Context& ctx);

	Window window_;
	Renderer renderer_;
	Resource resource_;
	Director director_;
	bool is_initialized_;
};

}