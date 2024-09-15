#pragma once

#include "Mesh.h"
#include "RendererModel.h"
#include "RenderModel.h"
#include "SomeRenderContext.h"
#include "WorldComponent.h"
#include "WorldSizeable.h"
#include <memory>

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 9/6/24
 */
namespace PJ {
    class RenderMaterial;
    class RenderContextModel;
    class RenderModel;
    class SomeTexture;

    class SomeRenderer : public WorldComponent<>, public WorldSizeable {
    public:
        using This = SomeRenderer;
        using BuildMeshFunc = RendererModel::BuildMeshFunc;

        /// Defines model for renderer
        RendererModel model;

        SomeRenderer(Vector3 worldSize);

        /// Create models to send to the render engine for a render
        virtual VectorList<RenderModel> MakeRenderModels();

        This& SetBuildMeshFunc(BuildMeshFunc buildMeshFunc) {
            model.SetBuildMeshFunc(buildMeshFunc);
            return *this;
        }

        /// Override to set render color
        virtual void SetColor(Color color) {
            model.SetColor(color);
        }

        Color GetColor() {
            return model.Color();
        }

        // MARK: WorldSizeable

        std::optional<Vector3> WorldSize() const override {
            return model.WorldSize();
        }

        void SetWorldSize(Vector3 value) override {
            model.SetWorldSize(value);
        }
    };
} // namespace PJ
