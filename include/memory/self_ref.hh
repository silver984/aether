#pragma once

namespace aether {

template <typename T>
class wref;

template <typename T>
class sref;

template <typename T>
class self_ref {
	friend class sref<T>;

public:
	constexpr self_ref() = default;
	virtual ~self_ref()  = default;

protected:
	[[nodiscard]] sref<T> this_() const {
		return weak_this_.lock();
	}

private:
	wref<T> weak_this_;
};

} // namespace aether