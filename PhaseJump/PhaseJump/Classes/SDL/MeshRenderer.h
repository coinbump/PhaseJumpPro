#ifndef PJMESHRENDERER_H
#define PJMESHRENDERER_H

#include "SomeRenderer.h"
#include <memory>

/*
 BUG: vertices aren't adjusted for object's position
 Tested and works
 CODE REVIEW: 4/17/23
 */
namespace PJ {
    class RenderMaterial;
    class RenderMesh;

    /// Renders a mesh
    class MeshRenderer : public SomeRenderer {
    public:
        RenderMesh mesh;
        std::shared_ptr<RenderMaterial> material;

        // MARK: SomeRenderer

        void RenderInto(RenderIntoModel model) override;
    };
}

#endif
