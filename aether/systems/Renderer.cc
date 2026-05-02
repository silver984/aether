#include <aether/systems/Renderer.hh>
#include <raylib.h>
#include <fmt/format.h>

namespace ae {

// private
Renderer::Renderer() = default;

// private
Renderer::~Renderer() = default;

size<int> Renderer::draw_size() const {
	return {
		.width = GetRenderWidth(),
		.height = GetRenderHeight()
	};
}

// private
void Renderer::start_draw() const {
	BeginDrawing();
	// clip bounds
	auto _draw_size_ = draw_size();
	BeginScissorMode(0, 0, _draw_size_.width, _draw_size_.height);
	ClearBackground(BLACK);
}

// private
void Renderer::end_draw() const {
	EndScissorMode();
	EndDrawing();
}

#ifdef AETHER_DEBUG
// private
void Renderer::draw_debug(Context const& ctx) const {
	static std::string debug_text;
	debug_text = fmt::format("FPS: {}", ctx.running_fps());
	DrawText(debug_text.c_str(), 5, 5, 10, WHITE);
}
#endif

}