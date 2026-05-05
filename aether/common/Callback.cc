#include <aether/common/Callback.hh>

namespace ae {

Callback::Callback(void* owner, std::function<void(void*)>&& fn) :
    owner_(owner),
    fn_(std::move(fn))
{}

Callback::~Callback() = default;

bool Callback::is_expired() const {
    return !owner_;
}

void Callback::operator()() const {
    if (owner_) {
        fn_(owner_);
    }
}

}