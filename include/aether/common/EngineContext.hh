#pragma once

namespace ae {

class Funkin;
class Window;

class EngineContext final {
	friend class Funkin;
private:
	explicit EngineContext(Window* window_ptr);
	~EngineContext();

public:
	EngineContext(EngineContext const&) = delete;
	EngineContext(EngineContext&&) = delete;
	EngineContext& operator =(EngineContext const&) = delete;
	EngineContext& operator =(EngineContext&&) = delete;

	[[nodiscard]] Window const* window() const;

private:
	Window* window_;
};

}