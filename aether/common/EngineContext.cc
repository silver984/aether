#include <aether/common/EngineContext.hh>

namespace ae {

// private
EngineContext::EngineContext(Window* window_ptr) :
	window_(window_ptr)
{}

// private
EngineContext::~EngineContext() = default;

Window const* EngineContext::window() const {
	return window_;
}

}