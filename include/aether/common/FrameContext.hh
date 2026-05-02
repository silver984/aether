#pragma once

namespace ae {

class Funkin;

class FrameContext final {
	friend class Funkin;
private:
	FrameContext();
	~FrameContext();

public:
	FrameContext(FrameContext const&) = delete;
	FrameContext(FrameContext&&) = delete;
	FrameContext& operator =(FrameContext const&) = delete;
	FrameContext& operator =(FrameContext&&) = delete;

	[[nodiscard]] float delta_time() const;
};

}