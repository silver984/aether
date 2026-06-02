#pragma once
#include <objects/Node.hh>

class TestState : public aether::NodeIdentity<TestState> {
	friend class aether::NodeIdentity<TestState>;

public:
	TestState(aether::Context const& ctx);
	~TestState();

protected:
	bool init() override;
	void update(float dt) override;

private:
	static constexpr std::string_view TYPE_ = "TestState";
};