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

        /// Override to reset internal renderer states
        virtual void Reset() {}

        /// Create models to send to the render engine for a render
        virtual VectorList<RenderModel> MakeRenderModels();

        This& SetBuildMeshFunc(BuildMeshFunc buildMeshFunc) {
            model.SetBuildMeshFunc(buildMeshFunc);
            return *this;
        }

        void SetColor(Color color) {
            model.SetColor(color);
        }

        void SetAlpha(float value) {
            model.SetAlpha(value);
        }

        Color GetColor() {
            return model.Color();
        }

        /// Some renderers have a limited time span (animations)
        virtual bool IsRenderFinished() const {
            return false;
        }

        // MARK: WorldSizeable

        Vector3 WorldSize() const override {
            return model.WorldSize();
        }

        void SetWorldSize(Vector3 value) override {
            model.SetWorldSize(value);
        }
    };
} // namespace PJ
