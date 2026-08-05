#pragma once
#include <cstddef>
#include <debug/log.hh>
#include <functional>
#include <lua/manager.hh>
#include <string_view>
#include <type_traits>
#include <vector>

namespace aether::lua {

// returns the same type as `impl`, assuming that its a function pointer
template <typename obj, typename fn, typename... va>
auto hookchain(obj* ptr, std::string_view function_name, fn impl, va... args) {
	auto& hook_map = manager::hook_map();
	auto const it  = hook_map.find(function_name);

	if (it == hook_map.end()) {
		return std::invoke(impl, ptr, args...);
	}

	using ret = std::invoke_result_t<fn, obj*, va...>;
	std::vector<size_t> invalid_indices;

	auto current_callbacks = it->second.callbacks;
	auto chain             = [&impl, &current_callbacks, &invalid_indices, &function_name](size_t i, auto&& self, obj* lptr,
	                                                                                       va... chain_args) -> ret {
		if (i >= current_callbacks.size()) {
			return std::invoke(impl, lptr, chain_args...);
		}

		auto& callback = current_callbacks[i];
		auto prcd      = [i, &self, lptr](va... prcd_args) -> ret {
			return self(i + 1, self, lptr, prcd_args...);
		};

		sol::protected_function_result result = callback(lptr, prcd, chain_args...);

		if (!result.valid()) {
			sol::error e = result;
			AETHER_ENGINE_ERRORLOG("Invalid hook for \"{}\" ? index: {}, what: {}", function_name, i, e.what());
			invalid_indices.emplace_back(i);
			return std::invoke(impl, lptr, chain_args...);
		}

		if constexpr (!std::is_void_v<ret>) {
			try {
				return result.get<ret>();
			} catch (sol::error const& e) {
				AETHER_ENGINE_ERRORLOG("Missing hook return value for \"{}\" ? index: {}, what: {}", function_name, i,
				                       e.what());
				invalid_indices.emplace_back(i);
				return std::invoke(impl, lptr, chain_args...);
			}
		}
	};

	auto cleanup = [&it, &invalid_indices, &hook_map, &function_name]() -> void {
		auto& callbacks = it->second.callbacks;
		for (size_t i : invalid_indices) {
			callbacks.erase(callbacks.begin() + i);
			AETHER_ENGINE_DEBUGLOG("Removed hook for \"{}\" at index {}", function_name, i);
		}
		if (callbacks.empty()) {
			hook_map.erase(it);
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

} // namespace aether::lua