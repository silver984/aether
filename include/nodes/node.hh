#pragma once
#include <concepts>
#include <cstddef>
#include <math/mat3.hh>
#include <math/rgba.hh>
#include <math/size.hh>
#include <math/vec2.hh>
#include <string>
#include <string_view>
#include <util/ref.hh>
#include <utility>
#include <vector>

namespace aether {

class context;
class scene;
// class camera;

class node {
	friend class scene;
	// friend class camera;

public:
	node(context const& ctx);
	virtual ~node();

	template <std::derived_from<node> T, typename... va>
	[[nodiscard]] static ref<T> create(context const& ctx, va&&... args) {
		ref<T> ptr = new T(ctx, std::forward<va>(args)...);
		if (!ptr->init_node_()) {
			return nullptr;
		}
		return ptr;
	}

	// todo: fetch child

	bool add_child(ref<node> n);
	bool remove_child(ref<node> n);
	bool remove_child(node* n);
	void destroy_all();
	bool detach_from_parent();
	void activate();
	void deactivate();
	void schedule_draw();
	void unschedule_draw();
	[[nodiscard]] size_t child_count() const;
	[[nodiscard]] size_t recursed_child_count() const;
	[[nodiscard]] node* parent() const;
	void set_name(std::string_view name); // todo: better naming system
	[[nodiscard]] std::string_view name() const;
	void set_bounds(size<int> val);       // todo: set_width, set_height
	[[nodiscard]] size<int> bounds() const;
	[[nodiscard]] int width() const;
	[[nodiscard]] int height() const;
	void set_position(vec2<float> val);
	void set_position_x(float val);
	void set_position_y(float val);
	[[nodiscard]] vec2<float> position() const;
	void set_anchor(vec2<float> val); // todo: set_anchor_x, set_anchor_y
	[[nodiscard]] vec2<float> anchor() const;
	void set_scale(vec2<float> val);
	void set_scale(float val);
	void set_scale_x(float val);
	void set_scale_y(float val);
	[[nodiscard]] vec2<float> scale() const;
	void set_skew(vec2<float> val);
	[[nodiscard]] vec2<float> skew() const;
	void set_scroll_factor(vec2<float> val);
	[[nodiscard]] vec2<float> scroll_factor() const;
	void set_rotation(float val);
	[[nodiscard]] float rotation() const;
	void set_color(rgba val);
	[[nodiscard]] rgba color() const;
	void set_alpha(float val);
	[[nodiscard]] float alpha() const;
	void toggle_visibility(bool val);
	[[nodiscard]] bool is_visible() const;
	void set_time_scale(float val);
	[[nodiscard]] float time_scale() const;
	void toggle_flip(bool val);
	void toggle_flip_x(bool val);
	[[nodiscard]] bool is_flip_x() const;
	void toggle_flip_y(bool val);
	[[nodiscard]] bool is_flip_y() const;
	[[nodiscard]] std::vector<ref<node>> children() const;
	[[nodiscard]] scene* get_scene() const;

protected:
	virtual bool init_();
	virtual void update_(float dt);
	virtual void draw_(mat3 const& transform, rgba color);
	[[nodiscard]] context const& ctx_() const;

private:
	bool init_node_();
	void update_all_(float dt);
	void draw_all_();
	[[nodiscard]] bool has_ancestor_(node* n) const;
	void mark_transform_dirty_();
	void mark_rgba_dirty_();
	[[nodiscard]] mat3 calculate_transform_() const;
	[[nodiscard]] rgba calculate_combined_rgba_() const;

	context const& mctx_;
	scene* scene_;
	node* parent_;
	std::vector<ref<node>> children_;
	std::string name_;
	mat3 transform_;
	size<int> bounds_;
	vec2<float> position_;
	vec2<float> anchor_;
	vec2<float> scale_;
	vec2<float> skew_; // degrees
	vec2<float> scroll_factor_;
	float rotation_;   // degrees
	float time_scale_;
	rgba color_;
	rgba combined_color_;
	bool is_flip_x_;
	bool is_flip_y_;
	bool is_transform_dirty_;
	bool is_rgba_dirty_;
	bool is_active_;
	bool is_draw_scheduled_;
	bool is_visible_;
};

} // namespace aether