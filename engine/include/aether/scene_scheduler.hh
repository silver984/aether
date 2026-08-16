#pragma once
#include <aether/general.h>
#include <aether/ref.hh>
#include <aether/service.hh>

namespace aether {

class game;
class scene;

class scene_scheduler final : public service<scene_scheduler> {
	friend class game;

public:
	~scene_scheduler() noexcept override;
	void replace_scene(unique_ref<scene> new_scene);

private:
	scene_scheduler();
	void cleanup_();
	void update_scene_(float dt);
	void draw_scene_();
	[[nodiscard]] bool has_pending_scene_() const;

	unique_ref<scene> current_scene_;
	unique_ref<scene> pending_scene_;
};

} // namespace aether