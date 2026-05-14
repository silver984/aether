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

	[[nodiscard]] std::weak_ptr<Window> window_wref() const;
	[[nodiscard]] std::weak_ptr<Renderer> renderer_wref() const;
	[[nodiscard]] std::weak_ptr<Resource> resource_wref() const;
	[[nodiscard]] std::weak_ptr<Director> director_wref() const;
	[[nodiscard]] float delta_time() const;
	[[nodiscard]] float total_time() const;
	[[nodiscard]] uint32_t running_fps() const;

  private:
	void store_refs(std::weak_ptr<Window> window, std::weak_ptr<Renderer> renderer, std::weak_ptr<Resource> resource,
	                std::weak_ptr<Director> director);
	void update_frame_ctx();

	std::weak_ptr<Window> window_wref_;
	std::weak_ptr<Renderer> renderer_wref_;
	std::weak_ptr<Resource> resource_wref_;
	std::weak_ptr<Director> director_wref_;
	float total_time_;
	uint32_t running_fps_;
};

} // namespace ae

#endif