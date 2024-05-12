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
    struct Mesh;

    /// Renders a single texture as a sprite
    class SpriteRenderer: public SomeRenderer {
    protected:
        Mesh mesh;

    public:
        bool flipX = false;
        bool flipY = false;

        // FUTURE: float pixelsPerUnit = 1.0f;

        SP<RenderMaterial> material;

        SpriteRenderer(SP<SomeTexture> texture);

        void RenderInto(RenderIntoModel model) override;
        Vector2 Size() const;

        std::optional<Vector3> WorldSize() const override {
            auto size = Size();
            return Vector3(size.x, size.y, 0);
        }

        void SetColor(Color color) override;
    };
}

#endif
