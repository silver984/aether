#pragma once
#include <aether/common/pointers.hh>
#include <functional>
#include <utility>

namespace ae {

class Callback {
public:
    Callback() = delete;
    Callback(void* owner, std::function<void(void*)>&& fn);
    ~Callback();

    bool is_expired() const;

    void operator()() const;

private:
    void* owner_;
    std::function<void(void*)> fn_;
};

}