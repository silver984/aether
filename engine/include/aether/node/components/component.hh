#pragma once
#include <aether/ref.hh>
#include <concepts>

namespace aether {

class node;
class context;

// used for `node_component` only
enum class schedule_level : uint8_t {
	none   = 0,
	update = 1 << 0,
	visit  = 1 << 1,
	draw   = 1 << 2,
};

constexpr schedule_level operator|(schedule_level a, schedule_level b) noexcept {
	using T = std::underlying_type_t<schedule_level>;
	return (schedule_level)((T)a | (T)b);
}

constexpr schedule_level operator&(schedule_level a, schedule_level b) noexcept {
	using T = std::underlying_type_t<schedule_level>;
	return (schedule_level)((T)a & (T)b);
}

constexpr schedule_level operator~(schedule_level value) noexcept {
	using T = std::underlying_type_t<schedule_level>;
	return (schedule_level)(~((T)value));
}

constexpr schedule_level& operator|=(schedule_level& a, schedule_level b) noexcept {
	return a = a | b;
}

constexpr schedule_level& operator&=(schedule_level& a, schedule_level b) noexcept {
	return a = a & b;
}

class node_component {
	friend class node;

public:
	node_component(context const& ctx, strong_ref<node> n);
	virtual ~node_component();

	template <typename T, typename... Args>
	        requires std::derived_from<T, node_component>
	[[nodiscard]] static unique_ref<T> create(context const& ctx, strong_ref<node> n, Args&&... args) noexcept {
		auto out = unique_ref<T>::create(ctx, n, std::forward<Args>(args)...);
		if (!out->init_interface_()) {
			return nullptr;
		}
		return out;
	}

	void schedule(schedule_level level) noexcept;
	void unschedule(schedule_level level) noexcept;
	bool is_scheduled(schedule_level level) const noexcept;

protected:
	virtual bool init_() noexcept;
	virtual void update_(float dt) noexcept;
	virtual void visit_() noexcept;
	virtual void draw_() noexcept;

	context const& ctx_;
	weak_ref<node> weak_node_;

private:
	bool init_interface_() noexcept;

	float time_scale_;

	schedule_level scheduled_;
};

template <typename T>
concept node_component_type = std::derived_from<T, node_component> && !std::same_as<T, node_component>;

} // namespace aether