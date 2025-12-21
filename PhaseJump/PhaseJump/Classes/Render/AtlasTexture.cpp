#include "AtlasTexture.h"
#include "TextureAtlas.h"

using namespace std;
using namespace PJ;

AtlasTexture::AtlasTexture(Config const& config) :
    Base(config.base),
    parent(config.parent) {}

Texture* AtlasTexture::RenderTexture() const {
    GUARDR(parent, {})
    return parent->texture.get();
}

uint32_t AtlasTexture::RenderId() const {
    GUARDR(parent && parent->texture, 0)
    return parent->texture->RenderId();
}
