#ifndef __AETHER_MATH_RECT_HH__
#define __AETHER_MATH_RECT_HH__

#include <aether/math/numeric_t.hh>

namespace ae {

template<numeric_t T>
struct rect {
    T x = T{0};
    T y = T{0};
    T width = T{0};
    T height = T{0};

    // TODO: operators
};

}

#endif