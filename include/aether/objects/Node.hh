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
	[[nodiscard]] size_t count() const;
	[[nodiscard]] float world_rotation() const;
	[[nodiscard]] vec2<float> world_position() const;
	[[nodiscard]] vec2<float> world_scale() const;
	[[nodiscard]] size<float> world_size() const;
	[[nodiscard]] float world_alpha() const;
	[[nodiscard]] size<float> bounds() const;
	[[nodiscard]] wptr<Node> parent() const;
	void set_name(std::string_view name);
	[[nodiscard]] std::string_view name() const;
	[[nodiscard]] virtual std::string_view type() const;

	rgb color;
	vec2<float> pos;
	vec2<float> anchor;
	vec2<float> scale;
	vec2<float> skew; // degrees
	float rotation; // degrees
	float alpha;
	float time_scale;
	bool is_visible;
	bool is_active;

protected:
	virtual bool init(Context const& ctx);
	virtual void update(Context const& ctx, float dt);
	virtual void draw(Context const& ctx) const;
	mat3 world_transform() const;

	size<float> bounds_;

private:
	bool base_init(Context const& ctx);
	void base_update(Context const& ctx, float dt);
	void base_draw(Context const& ctx) const;
	bool has_ancestor(sptr<Node> const& vessel) const;
	void mark_dirty();
	void on_dirty(Context const& ctx);

	wptr<Node> parent_;
	std::vector<sptr<Node>> children_;
	std::string name_;
	mat3 local_transform_;
	mat3 world_transform_;
	float world_alpha_;
	float last_rotation_;
	float last_alpha_;
	size<float> last_bounds_;
	vec2<float> last_pos_;
	vec2<float> last_anchor_;
	vec2<float> last_scale_;
	vec2<float> last_skew_;
	bool is_dirty_;
	bool is_initialized_;
};

}