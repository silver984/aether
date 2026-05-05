#pragma once
#include <memory>
#include <aether/common/Context.hh>
#include <aether/objects/Node.hh>

namespace ae {

class Funkin;

class Director final {
    friend class Funkin;
public:
    Director();
    ~Director();
    Director(Director const&) = delete;
    Director(Director&&) = delete;
    Director& operator=(Director const&) = delete;
    Director& operator=(Director&&) = delete;

    void switch_state(std::shared_ptr<Node>&& new_state);

private:
    void update_current_state(Context const& ctx);
    void draw_current_state(Context const& ctx);
    void move_pending_state();

    std::shared_ptr<Node> current_state_;
    std::shared_ptr<Node> pending_state_;
};

}