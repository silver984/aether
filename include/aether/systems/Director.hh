#pragma once
#include <aether/common/pointers.hh>
#include <aether/common/Context.hh>
#include <aether/objects/Node.hh>

namespace ae {

class Funkin;

class Director final {
    friend class Funkin;
private:
    Director();
    ~Director();

public:
    Director(Director const&) = delete;
    Director(Director&&) = delete;
    Director& operator=(Director const&) = delete;
    Director& operator=(Director&&) = delete;

    void switch_state(sptr<Node>&& new_state);

private:
    void update_current_state(Context const& ctx);
    void draw_current_state(Context const& ctx);

    sptr<Node> current_state_;
    sptr<Node> pending_state_;
};

}