#include <aether/internal/rl.hh>
#include <aether/math/vec2.hh>
#include <raylib.h>
#include <external/glfw/include/GLFW/glfw3.h>
#include <numeric>

namespace {

void set_window_aspect_ratio(GLFWwindow* const& window, ae::size<int> const& resolution) {
	int gcd = std::gcd(resolution.width, resolution.height);
	
	ae::vec2<int> aspect = {
		.x = resolution.width / gcd,
		.y = resolution.height / gcd,
	};

	if (aspect.x <= 0) {
		aspect.x = 1;
	}

	if (aspect.y <= 0) {
		aspect.y = 1;
	}

	glfwSetWindowAspectRatio(window, aspect.x, aspect.y);
}

}

namespace ae::rl {

bool init_window(char const* title, size<int> const& resolution, int fps) {
	SetTraceLogCallback([](int, char const*, va_list) {});

	SetConfigFlags(
		ConfigFlags::FLAG_WINDOW_RESIZABLE |
		ConfigFlags::FLAG_WINDOW_ALWAYS_RUN |
		ConfigFlags::FLAG_WINDOW_TRANSPARENT
	);

	InitWindow(resolution.width, resolution.height, title);

	if (!IsWindowReady()) {
		// TODO: log error
		return false;
	}

	set_window_aspect_ratio(glfwGetCurrentContext(), resolution);
	SetTargetFPS(fps);
	SetExitKey(KeyboardKey::KEY_NULL);

	return true;
}

float delta_time() {
	return GetFrameTime();
}

}