#pragma once
#include <memory>
#include <objects/abstract/Scene.hh>

class TestScene final : public aether::SceneIdentity<TestScene> {
	friend class aether::SceneIdentity<TestScene>;

public:
	TestScene(aether::Context const& ctx);
	~TestScene() override;

protected:
	bool init() override;

private:
	static constexpr std::string_view NAME_ = "TestScene";
};