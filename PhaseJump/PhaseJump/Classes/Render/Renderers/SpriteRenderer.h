#ifndef PJSPRITERENDERER_H
#define PJSPRITERENDERER_H

#include "SomeRenderer.h"
#include "SomeTexture.h"
#include <memory>

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 4/21/23
 */
namespace PJ {
    class RenderMaterial;
    class RenderMesh;

    /// Renders a single texture as a sprite
    class SpriteRenderer: public SomeRenderer {
    protected:
        RenderMesh mesh;

    public:
        bool flipX = false;
        bool flipY = false;

        // FUTURE: float pixelsPerUnit = 1.0f;

        SP<RenderMaterial> material;

        SpriteRenderer(SP<SomeTexture> texture);

        void RenderInto(RenderIntoModel model) override;
    };
}

#endif
