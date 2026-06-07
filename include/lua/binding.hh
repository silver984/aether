#pragma once
#include <sol/state_view.hpp>

namespace aether {

class app;

}

namespace aether::lua {

class binding {
	friend class app;

private:
	binding(sol::state_view lua);

public:
	virtual ~binding();

protected:
	virtual void expose() = 0;
	[[nodiscard]] sol::state_view& lua() const;

private:
	void populate();

	sol::state_view lua_;
};

} // namespace aether::lua