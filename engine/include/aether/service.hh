#pragma once
#include <stdexcept>

namespace aether {

template <typename Type>
class service {
public:
	service() {
		if (instance_s_) {
			throw std::logic_error("service instance already exists");
		}
		instance_s_ = static_cast<Type*>(this);
	}

	service(service const&) = delete;
	service(service&&)      = delete;

	virtual ~service() noexcept {
		instance_s_ = nullptr;
	}

	[[nodiscard]] static Type* instance() noexcept {
		return instance_s_;
	}

	service& operator=(service const&) = delete;
	service& operator=(service&&)      = delete;

private:
	static inline Type* instance_s_ = nullptr;
};

} // namespace aether