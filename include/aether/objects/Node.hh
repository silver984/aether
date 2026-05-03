#pragma once
#include <aether/math/vec2.hh>
#include <aether/math/size.hh>
#include <aether/math/mat3.hh>
#include <aether/graphics/rgb.hh>
#include <aether/common/pointers.hh>
#include <aether/common/Context.hh>
#include <vector>
#include <string>
#include <string_view>
#include <utility>
#include <cstddef>
#include <type_traits>

namespace ae {

class Node : public std::enable_shared_from_this<Node> {
public:
	Node();
	virtual ~Node();

	template<typename T, typename... va>
	requires std::is_base_of_v<Node, T>
	static sptr<T> create(Context const& ctx, va&&... args) {
		sptr<T> ptr = shared<T>(std::forward<va>(args)...);

		if (!ptr->base_init(ctx)) {
			ptr.reset();
			return nullptr;
		}

		return ptr;
	}

	void add(sptr<Node> vessel);
	void remove(sptr<Node> vessel);
	void destroy();
	void activate();
	void deactivate();
	void toggle_active(bool val);
	[[nodiscard]] bool is_active() const;
	[[nodiscard]] size_t count() const;
	[[nodiscard]] wptr<Node> parent() const;
	void set_name(std::string_view name);
	[[nodiscard]] std::string_view name() const;
	[[nodiscard]] virtual std::string_view type() const;
	void set_bounds(size<float> const& val);
	[[nodiscard]] size<float> bounds() const;
	void set_position(vec2<float> const& val);
	[[nodiscard]] vec2<float> position() const;
	void set_anchor(vec2<float> const& val);
	[[nodiscard]] vec2<float> anchor() const;
	void set_scale(vec2<float> const& val);
	[[nodiscard]] vec2<float> scale() const;
	void set_skew(vec2<float> const& val);
	[[nodiscard]] vec2<float> skew() const;
	void set_rotation(float val);
	[[nodiscard]] float rotation() const;
	void set_color(rgb const& val);
	[[nodiscard]] rgb color() const;
	void set_alpha(float val);
	[[nodiscard]] float alpha() const;
	void toggle_visibility(bool val);
	[[nodiscard]] bool is_visible() const;
	void set_time_scale(float val);
	[[nodiscard]] float time_scale() const;

protected:
	virtual bool init(Context const& ctx);
	virtual void update(Context const& ctx, float dt);
	virtual void draw(Context const& ctx, mat3 const& transform, float alpha) const;

private:
	bool base_init(Context const& ctx);
	void base_update(Context const& ctx, float dt);
	void base_draw(Context const& ctx) const;
	[[nodiscard]] bool has_ancestor(sptr<Node> const& vessel) const;
	void mark_dirty();
	void on_dirty(Context const& ctx);

	wptr<Node> parent_;
	std::vector<sptr<Node>> children_;
	std::string name_;
	rgb color_;
	mat3 transform_;
	size<float> bounds_;
	vec2<float> position_;
	vec2<float> anchor_;
	vec2<float> scale_;
	vec2<float> skew_; // degrees
	float rotation_; // degrees
	float alpha_;
	float world_alpha_;
	float time_scale_;
	bool is_dirty_;
	bool is_active_;
	bool is_visible_;
	bool is_initialized_;
};

}