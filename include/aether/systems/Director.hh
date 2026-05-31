#pragma once
#include <aether/objects/Node.hh>
#include <memory>

namespace ae {

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
	void try_cleanup();
	void release_current_state();
	void release_pending_state();
	void update_current_state(float dt);
	void draw_current_state();
	void move_pending_state();

	std::shared_ptr<Node> current_state_;
	std::shared_ptr<Node> pending_state_;
};

} // namespace ae