#pragma once
#include <cstddef>
#include <debug/log.hh>
#include <functional>
#include <lua/manager.hh>
#include <string_view>
#include <type_traits>
#include <vector>

namespace aether::lua {

template <typename obj, typename fn, typename... va>
auto hookchain(obj* ptr, std::string_view function_name, fn impl, va... args) {
	auto& hook_map      = manager::hook_map();
	auto const iterator = hook_map.find(function_name);

	if (iterator == hook_map.end()) {
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

	auto cleanup = [&iterator, &invalid_indices, &hook_map]() -> void {
		auto& rcallbacks = iterator->second.callbacks;
		for (size_t i : invalid_indices) {
			rcallbacks.erase(rcallbacks.begin() + i);
		}
		if (rcallbacks.empty()) {
			hook_map.erase(iterator);
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