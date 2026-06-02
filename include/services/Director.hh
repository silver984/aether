#pragma once
#include <memory>
#include <objects/Node.hh>

namespace aether {

class Aether;

class Director final {
	friend class Aether;

private:
	Director();

public:
	~Director();
	Director(Director const&)            = delete;
	Director(Director&&)                 = delete;
	Director& operator=(Director const&) = delete;
	Director& operator=(Director&&)      = delete;

	void switch_state(std::shared_ptr<Node>&& new_state);

private:
	void cleanup();
	void update_current_state(float dt);
	void draw_current_state();
	[[nodiscard]] bool has_pending_state() const;

	std::shared_ptr<Node> current_state_;
	std::shared_ptr<Node> pending_state_;
};

} // namespace ae