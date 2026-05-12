#include <aether/graphic/texture_deleter.hh>
#include <aether/common/log.hh>
#include <raylib.h>

namespace ae {

void texture_deleter::operator()(Texture* ptr) const {
	if (ptr && ptr->id > 0) {
		UnloadTexture(*ptr);
		tracelog("Unloaded texture ({}) | OpenGL id: {}", fmt::ptr(ptr), ptr->id);
	}

	delete ptr;
}

}