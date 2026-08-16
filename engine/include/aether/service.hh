#pragma once
#include <stdexcept>

namespace aether {

template <typename Type>
class service {
public:
	service() noexcept      = default;
	service(service const&) = delete;
	service(service&&)      = delete;

	virtual ~service() noexcept {
		hide_();
	}

	[[nodiscard]] static Type* instance() noexcept {
		return instance_s_;
	}

	service& operator=(service const&) = delete;
	service& operator=(service&&)      = delete;

protected:
	void expose_() {
		Type* ptr = dynamic_cast<Type*>(this);

		if (instance_s_ == ptr) {
			return;
		}

		if (instance_s_) {
			throw std::logic_error("service instance already exists");
		}

		instance_s_ = ptr;
	}

	void hide_() noexcept {
		instance_s_ = nullptr;
	}

private:
	static inline Type* instance_s_ = nullptr;
};

} // namespace aether