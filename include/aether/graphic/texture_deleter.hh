#ifndef __AETHER_GRAPHIC_TEXTURE_DELETER_HH__
#define __AETHER_GRAPHIC_TEXTURE_DELETER_HH__

struct Texture;

namespace ae {

struct texture_deleter {
	void operator()(Texture* ptr) const;
};

}

#endif