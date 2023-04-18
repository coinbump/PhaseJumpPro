#ifndef PJSOMERENDERER_H
#define PJSOMERENDERER_H

#include "WorldComponent.h"
#include "SomeRenderContext.h"
#include "Bounds.h"
#include "RenderMesh.h"
#include <memory>

#include "Axis.h"

namespace PJ
{
    class RenderMaterial;

    //    class MeshRenderer : public SomeRenderer {
//    public:
//        using Base = SomeRenderer;
//
//        RenderMesh renderMesh;
//
//        // This is backwards?
////        std::shared_ptr<SomeRenderMeshBuilder> meshBuilder;
////
////        void Build(bool rebuild) {
////            if (rebuild) {
////                renderMesh = nullptr;
////            }
////
////            if (renderMesh || nullptr == meshBuilder) { return; }
////            renderMesh = meshBuilder->BuildRenderMesh();
////        }
//
//        void Awake() override {
//            Base::Awake();
//        }
//
//        // TODO: handle render
//    };

    class SomeRenderer : public WorldComponent {
    public:
        // (OPTIONAL) Material for render
        std::shared_ptr<RenderMaterial> material;

        virtual void RenderInto(RenderIntoModel model) = 0;
    };

    // TODO:
//    class MeshRenderer : public SomeRenderer {
//        std::shared_ptr<RenderMesh> mesh;
//
//        void RenderInto(RenderIntoModel model) override {
//            glRenderEngine->VertexAttributePointer(0, 3, GL_FLOAT, 0, vertices.data());
//            glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)vertices.size());
//        }
//    };

    /// Render based on a function (used for imGui)
    class FuncRenderer : public SomeRenderer {
    public:
        std::function<void(RenderIntoModel)> render;

        FuncRenderer(std::function<void(RenderIntoModel)> render) : render(render) {
        }

        void RenderInto(RenderIntoModel model) override;
    };
}

#endif
