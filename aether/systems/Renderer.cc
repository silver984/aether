#include <aether/systems/Renderer.hh>
#include <raylib.h>

namespace ae {

// private
Renderer::Renderer() = default;

// private
Renderer::~Renderer() = default;

// private
void Renderer::start_draw() const {
	BeginDrawing();
	BeginScissorMode(0, 0, GetRenderWidth(), GetRenderHeight());
	ClearBackground(BLACK);
}

// private
void Renderer::end_draw() const {
	// DrawText(debug_text.c_str(), text_border_padding, text_border_padding, text_size, WHITE);
	EndScissorMode();
	EndDrawing();
}

}