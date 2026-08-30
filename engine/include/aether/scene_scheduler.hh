#pragma once
#include <aether/general.h>
#include <aether/ref.hh>

namespace aether {

class game;
class scene;

class scene_scheduler final {
	friend class game;

public:
	~scene_scheduler() = default;
	void replace_scene(unique_ref<scene> new_scene);

private:
	scene_scheduler() = default;
	void cleanup_();
	void update_scene_(float dt);
	void draw_scene_();
	[[nodiscard]] bool has_pending_scene_() const { return pending_scene_ != nullptr; }

	unique_ref<scene> current_scene_;
	unique_ref<scene> pending_scene_;
};

} // namespace aether