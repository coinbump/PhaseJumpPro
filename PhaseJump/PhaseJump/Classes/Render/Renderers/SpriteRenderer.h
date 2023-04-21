#ifndef PJSPRITERENDERER_H
#define PJSPRITERENDERER_H

#include "SomeRenderer.h"
#include <memory>

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 4/21/23
 */
namespace PJ {
    class RenderMaterial;
    class RenderMesh;
    class RenderTexture;

    /// Renders a single texture as a sprite
    class SpriteRenderer: public SomeRenderer {
    protected:
        RenderMesh mesh;

    public:
        bool flipX = false;
        bool flipY = false;

        std::shared_ptr<RenderMaterial> material;

        SpriteRenderer(RenderTexture texture);

        void RenderInto(RenderIntoModel model) override;
    };
}

#endif
