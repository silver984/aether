#ifndef __AETHER_COMMON_CONTEXT_HH__
#define __AETHER_COMMON_CONTEXT_HH__

#include <cstdint>
#include <memory>

namespace ae {

class Funkin;
class Window;
class Renderer;
class Resource;
class Director;

class Context final {
	friend class Funkin;
private:
	Context();

public:
	~Context();

	Context(Context const&) = delete;
	Context(Context&&) = delete;
	Context& operator=(Context const&) = delete;
	Context& operator=(Context&&) = delete;

	[[nodiscard]] std::weak_ptr<Window> window() const;
	[[nodiscard]] std::weak_ptr<Renderer> renderer() const;
	[[nodiscard]] std::weak_ptr<Resource> resource() const;
	[[nodiscard]] std::weak_ptr<Director> director() const;
	[[nodiscard]] float delta_time() const;
	[[nodiscard]] float total_time() const;
	[[nodiscard]] uint32_t running_fps() const;

private:
	void store_refs(
		std::weak_ptr<Window> window_ptr,
		std::weak_ptr<Renderer> renderer_ptr,
		std::weak_ptr<Resource> resource_ptr,
		std::weak_ptr<Director> director_ptr
	);

	void update_frame_ctx();

	std::weak_ptr<Window> window_;
	std::weak_ptr<Renderer> renderer_;
	std::weak_ptr<Resource> resource_;
	std::weak_ptr<Director> director_;
	float total_time_;
	uint32_t running_fps_;
};

}

#endif