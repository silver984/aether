#pragma once
#include <memory>
#include <utility>

namespace ae {

template<typename T>
using sptr = std::shared_ptr<T>;

template<typename T>
using wptr = std::weak_ptr<T>;

template<typename T>
using uptr = std::unique_ptr<T>;

template<typename T, typename... va>
sptr<T> shared(va&&... args) {
	return std::make_shared<T>(std::forward<va>(args)...);
}

template<typename T, typename... va>
uptr<T> unique(va&&... args) {
	return std::make_unique<T>(std::forward<va>(args)...);
}

}