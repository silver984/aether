#include <context.hh>
#include <debug/log.hh>
#include <lua/manager.hh>
#include <nodes/animated_sprite.hh>
#include <testscene.hh>
#include <window.hh>

using namespace aether;

testscene::testscene(context const& ctx)
        : scene(ctx) {
}

testscene::~testscene() = default;

bool testscene::init_() {
	auto const& ctx  = ctx_();
	auto window_size = ctx.get_window().target_size();
	auto animated    = node::create<animated_sprite>(
	        ctx, animated_sprite::descriptor{.image_file = "resources/bf.png", .data_file = "resources/bf.xml", .fps = 12});

	if (!animated) {
		return false;
	}

	animated->play_animation("idle", {.loop = true});
	animated->set_scale(0.7f);
	animated->toggle_flip_x(true);
	animated->set_position(vec2<float>(400.f, window_size.height / 2.f));
	add(animated);
	activate();

	return true;
}

template <typename obj, typename fn, typename... va>
auto hookchain(obj* ptr, std::string_view function_name, fn impl, va... args) {
	auto& lua_hooks     = lua::manager::hook_map();
	auto const iterator = lua_hooks.find(function_name);

	if (iterator == lua_hooks.end()) {
		return std::invoke(impl, ptr, args...);
	}

	using ret = std::invoke_result_t<fn, obj*, va...>;
	std::vector<size_t> invalid_indices;

	auto ccallbacks = iterator->second.callbacks;
	auto chain      = [&impl, &ccallbacks, &invalid_indices](size_t i, auto&& self, obj* lptr, va... chain_args) -> ret {
		if (i >= ccallbacks.size()) {
			return std::invoke(impl, lptr, chain_args...);
		}

		auto& callback = ccallbacks[i];
		auto prcd      = [i, &self, lptr](va... prcd_args) -> ret {
			return self(i + 1, self, lptr, prcd_args...);
		};

		sol::protected_function_result result = callback(lptr, prcd, chain_args...);

		if (!result.valid()) {
			sol::error e = result;
			AETHER_ERRORLOG("Invalid hook callback | what: {}", e.what());
			invalid_indices.emplace_back(i);
			return std::invoke(impl, lptr, chain_args...);
		}

		if constexpr (!std::is_void_v<ret>) {
			if (result.return_count() <= 0) {
				return std::invoke(impl, lptr, chain_args...);
			}
			return result.get<ret>();
		}
	};

	auto cleanup = [&iterator, &invalid_indices, &lua_hooks]() -> void {
		auto& rcallbacks = iterator->second.callbacks;
		for (size_t i : invalid_indices) {
			rcallbacks.erase(rcallbacks.begin() + i);
		}
		if (rcallbacks.empty()) {
			lua_hooks.erase(iterator);
		}
	};

	if constexpr (!std::is_void_v<ret>) {
		ret out = chain(0, chain, ptr, args...);
		cleanup();
		return out;
	} else {
		chain(0, chain, ptr, args...);
		cleanup();
	}
}

float testscene::test_() {
	return hookchain(this, "testscene:test_", &testscene::test_impl_);
}

float testscene::test_impl_() {
	return 1.f;
}

void testscene::update_(float dt) {
	hookchain(this, "testscene:update_", &testscene::update_impl_, dt);
	return;
}

void testscene::update_impl_(float dt) {
	for (auto& child : root()->children()) {
		child->set_rotation(child->rotation() + (90.f * dt));
	}
	AETHER_INFOLOG("{}", test_());
}