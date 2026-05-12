#include <aether/graphic/texture_atlas.hh>
#include <aether/common/log.hh>

namespace ae {

void texture_atlas::deleter::operator()(texture_atlas* ptr) const {
	delete ptr;
	tracelog("Freed texture atlas ({})", fmt::ptr(ptr));
}

}