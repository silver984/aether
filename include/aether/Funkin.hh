#pragma once
#include <aether/math/size.hh>
#include <aether/systems/Window.hh>
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
	
private:
	void shutdown();

	Window window_;
	bool is_initialized_;
};

}