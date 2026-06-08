#pragma once
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <math/mat3.hh>
#include <math/rgba.hh>
#include <math/size.hh>
#include <math/vec2.hh>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace aether {

class context;
class scene;
class camera;

class node : public std::enable_shared_from_this<node> {
	friend class scene;
	friend class camera;

    public:
	node(context const& ctx);
	virtual ~node();

	template <typename derived, typename... va>
	        requires std::derived_from<derived, node>
	[[nodiscard]] static std::shared_ptr<derived> create(context const& ctx, va&&... args) {
		std::shared_ptr<derived> ptr = std::make_shared<derived>(ctx, std::forward<va>(args)...);

		if (!ptr->init_node()) {
			return nullptr;
		}

		return ptr;
	}

	template <typename derived>
	        requires std::derived_from<derived, node>
	[[nodiscard]] std::shared_ptr<derived> fetch_child_as(std::string_view name) {
		std::shared_ptr<node> node = fetch_child(name);
		return std::dynamic_pointer_cast<derived>(node);
	}

	bool add_child(std::shared_ptr<node> node);
	bool remove_child(std::shared_ptr<node> node);
	[[nodiscard]] std::shared_ptr<node> fetch_child(std::string_view name);
	void destroy_all();
	bool detach_from_parent();
	void activate();
	void deactivate();
	void schedule_draw();
	void unschedule_draw();
	[[nodiscard]] size_t child_count() const;
	[[nodiscard]] size_t recursed_child_count() const;
	[[nodiscard]] std::weak_ptr<node> parent() const;
	void set_name(std::string_view name); // todo: better naming system
	[[nodiscard]] std::string_view name() const;
	void set_bounds(size<int> val);       // todo: set_width, set_height
	[[nodiscard]] size<uint32_t> bounds() const;
	[[nodiscard]] uint32_t width() const;
	[[nodiscard]] uint32_t height() const;
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
	[[nodiscard]] std::vector<std::shared_ptr<node>> children() const;

    protected:
	virtual bool init();
	virtual void update(float dt);
	virtual void draw(mat3 const& transform, rgba color);
	[[nodiscard]] context const& ctx() const;
	[[nodiscard]] scene* fetch_scene() const;

    private:
	bool init_node();
	void update_all(float dt);
	void draw_all();
	[[nodiscard]] bool has_ancestor(std::shared_ptr<node> node) const;
	void mark_transform_dirty();
	void mark_rgba_dirty();
	[[nodiscard]] mat3 calculate_transform(std::weak_ptr<node> parent) const;
	[[nodiscard]] rgba calculate_combined_rgba(std::weak_ptr<node> parent) const;

	context const& ctx_;
	scene* scene_;
	std::vector<std::shared_ptr<node>> children_;
	std::weak_ptr<node> parent_;
	std::string name_;
	mat3 transform_;
	size<uint32_t> bounds_;
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