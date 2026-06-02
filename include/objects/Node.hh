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
#include <type_traits>
#include <utility>
#include <vector>

namespace aether {

class SceneScheduler;
class Context;

class Node : public std::enable_shared_from_this<Node> {
	friend class SceneScheduler;

public:
	Node(Context const& ctx);
	virtual ~Node();

	template <typename T, typename... va>
	    requires std::is_base_of_v<Node, T>
	static std::shared_ptr<T> create(Context const& ctx, va&&... args) {
		std::shared_ptr<T> ptr = std::make_shared<T>(ctx, std::forward<va>(args)...);

		if (!ptr->base_init()) {
			return nullptr;
		}

		return ptr;
	}

	void add_child(std::shared_ptr<Node> node);
	void remove_child(std::shared_ptr<Node> node);
	void destroy();
	void activate();
	void deactivate();
	void toggle_active(bool val);
	[[nodiscard]] bool is_active() const;
	void enable_draw();
	void disable_draw();
	void toggle_draw(bool val);
	[[nodiscard]] bool is_draw_enabled() const;
	[[nodiscard]] std::size_t child_count() const;
	[[nodiscard]] std::size_t recursed_child_count() const;
	[[nodiscard]] std::weak_ptr<Node> parent() const;
	void set_name(std::string_view name);
	[[nodiscard]] std::string_view name() const;
	[[nodiscard]] virtual std::string_view type() const;
	void set_bounds(size<int> val); // TODO: set_width, set_height
	[[nodiscard]] size<std::uint32_t> bounds() const;
	[[nodiscard]] std::uint32_t width() const;
	[[nodiscard]] std::uint32_t height() const;
	void set_position(vec2<float> val);
	void set_position_x(float val);
	void set_position_y(float val);
	[[nodiscard]] vec2<float> position() const;
	void set_anchor(vec2<float> val); // TODO: set_anchor_x, set_anchor_y
	[[nodiscard]] vec2<float> anchor() const;
	void set_scale(vec2<float> val);
	void set_scale(float val);
	void set_scale_x(float val);
	void set_scale_y(float val);
	[[nodiscard]] vec2<float> scale() const;
	void set_skew(vec2<float> val);
	[[nodiscard]] vec2<float> skew() const;
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
	[[nodiscard]] std::vector<std::shared_ptr<Node>> children() const;

protected:
	virtual bool init();
	virtual void update(float dt);
	virtual void draw(mat3 const& transform, rgba color);

	Context const& ctx_;

private:
	bool base_init();
	void base_update(float dt);
	void base_draw();
	[[nodiscard]] bool has_ancestor(std::shared_ptr<Node> node) const;
	void mark_transform_dirty();
	void mark_rgba_dirty();
	[[nodiscard]] mat3 calculate_transform(std::weak_ptr<Node> parent) const;
	[[nodiscard]] rgba calculate_combined_rgba(std::weak_ptr<Node> parent) const;

	std::vector<std::shared_ptr<Node>> children_;
	std::weak_ptr<Node> parent_;
	std::string name_;
	mat3 transform_;
	size<std::uint32_t> bounds_;
	vec2<float> position_;
	vec2<float> anchor_;
	vec2<float> scale_;
	vec2<float> skew_; // degrees
	float rotation_;   // degrees
	float time_scale_;
	rgba color_;
	rgba combined_color_;
	bool is_flip_x_;
	bool is_flip_y_;
	bool is_transform_dirty_;
	bool is_rgba_dirty_;
	bool is_active_;
	bool is_draw_enabled_;
	bool is_visible_;
	bool is_initialized_;
};

template <typename T>
class NodeIdentity : public Node {
public:
	using Node::Node;

	template <typename U, typename... va>
	static std::shared_ptr<U> create(Context const&, va&&...) = delete;

	std::string_view type() const override {
		return T::TYPE_;
	}
};

} // namespace aether