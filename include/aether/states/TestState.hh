#pragma once
#include <aether/objects/abstract/State.hh>
#include <aether/objects/Graphic.hh>
#include <memory>

class TestState : public ae::Node {
public:
	TestState();
	~TestState();

protected:
	bool init(ae::Context const& ctx) override;
	void update(ae::Context const& ctx, float dt) override;

private:
	float elapsed_;
	std::shared_ptr<ae::Graphic> grah_;
};
