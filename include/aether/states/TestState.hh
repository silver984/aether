#pragma once
#include <aether/objects/Node.hh>

class TestState : public ae::NodeIdentity<TestState> {
	friend class ae::NodeIdentity<TestState>;

public:
	TestState(ae::Context const& ctx);
	~TestState();

protected:
	bool init() override;
	void update(float dt) override;

private:
	static constexpr std::string_view TYPE_ = "TestState";
};