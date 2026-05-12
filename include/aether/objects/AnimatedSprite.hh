#ifndef __AETHER_OBJECTS_ANIMATEDSPRITE_HH__
#define __AETHER_OBJECTS_ANIMATEDSPRITE_HH__

#include <aether/objects/Node.hh>
#include <aether/graphic/texture_atlas.hh>

namespace ae {

class AnimatedSprite : public Node {
public:
	AnimatedSprite(Context const& ctx);
	~AnimatedSprite() override;
};

}

#endif