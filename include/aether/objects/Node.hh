#ifndef __AETHER_OBJECTS_NODE_HH__
#define __AETHER_OBJECTS_NODE_HH__

#include <aether/math/vec2.hh>
#include <aether/math/size.hh>
#include <aether/math/mat3.hh>
#include <aether/math/rgb.hh>
#include <aether/common/Context.hh>
#include <memory>
#include <vector>
#include <string>
#include <string_view>
#include <utility>
#include <cstddef>
#include <type_traits>

namespace ae {

class Director;

class Node : public std::enable_shared_from_this<Node> {
	friend class Director;
public:
	Node(Context const& ctx);
	virtual ~Node();

	template<typename T, typename... va>
	requires std::is_base_of_v<Node, T>
	static std::shared_ptr<T> create(Context const& ctx, va&&... args) {
		std::shared_ptr<T> ptr = std::make_shared<T>(ctx, std::forward<va>(args)...);

		if (!ptr->base_init()) {
			ptr.reset();
			return nullptr;
		}

		return ptr;
	}

	void add(std::shared_ptr<Node> node);
	void remove(std::shared_ptr<Node> node);
	void destroy();
	void activate();
	void deactivate();
	void toggle_active(bool val);
	[[nodiscard]] bool is_active() const;
	void enable_draw();
	void disable_draw();
	void toggle_draw(bool val);
	[[nodiscard]] bool is_draw_enabled() const;
	[[nodiscard]] size_t count() const;
	[[nodiscard]] std::weak_ptr<Node> parent() const;
	void set_name(std::string_view name);
	[[nodiscard]] std::string_view name() const;
	[[nodiscard]] virtual std::string_view type() const;
	void set_bounds(size<float> val); // TODO: set_width, set_height
	[[nodiscard]] size<float> bounds() const;
	void set_position(vec2<float> val); // TODO: set_position_x, set_position_y
	[[nodiscard]] vec2<float> position() const;
	void set_anchor(vec2<float> val); // TODO: set_anchor_x, set_anchor_y
	[[nodiscard]] vec2<float> anchor() const;
	void set_scale(vec2<float> val);
	void set_scale(float val);
	[[nodiscard]] vec2<float> scale() const;
	void set_skew(vec2<float> val);
	[[nodiscard]] vec2<float> skew() const;
	void set_rotation(float val);
	[[nodiscard]] float rotation() const;
	void set_color(rgb val);
	[[nodiscard]] rgb color() const;
	void set_alpha(float val);
	[[nodiscard]] float alpha() const;
	void toggle_visibility(bool val);
	[[nodiscard]] bool is_visible() const;
	void set_time_scale(float val);
	[[nodiscard]] float time_scale() const;

protected:
	virtual bool init();
	virtual void update(float dt);
	virtual void draw(mat3 const& transform, rgb color, float alpha) const;
	[[nodiscard]] Context const& context() const;

private:
	bool base_init();
	void base_update(float dt);
	void base_draw();
	[[nodiscard]] bool has_ancestor(std::shared_ptr<Node> node) const;
	void mark_transform_dirty();
	void mark_alpha_dirty();
	void mark_rgb_dirty();
	[[nodiscard]] mat3 calculate_transform(std::weak_ptr<Node> parent) const;
	[[nodiscard]] float calculate_combined_alpha(std::weak_ptr<Node> parent) const;
	[[nodiscard]] rgb calculate_combined_rgb(std::weak_ptr<Node> parent) const;

	Context const* const context_;
	std::weak_ptr<Node> parent_;
	std::vector<std::shared_ptr<Node>> children_;
	std::string name_;
	rgb color_;
	rgb combined_color_;
	mat3 transform_;
	size<float> bounds_;
	vec2<float> position_;
	vec2<float> anchor_;
	vec2<float> scale_;
	vec2<float> skew_; // degrees
	float rotation_; // degrees
	float alpha_;
	float combined_alpha_;
	float time_scale_;
	bool is_transform_dirty_;
	bool is_alpha_dirty_;
	bool is_rgb_dirty_;
	bool is_active_;
	bool is_draw_enabled_;
	bool is_visible_;
	bool is_initialized_;
};

}

#endif