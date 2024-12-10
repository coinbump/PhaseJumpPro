#pragma once

#include "Mesh.h"
#include "RenderModel.h"
#include "RenderTypes.h"
#include "Tags.h"
#include "Vector3.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 9/6/24
 */
namespace PJ {
    class RenderMaterial;

    /// Standard model for a renderer that renders a mesh
    class RendererModel {
    public:
        using This = RendererModel;

        using RendererFunc = std::function<void(This&)>;
        using BuildMeshFunc = std::function<Mesh(RendererModel const& model)>;
        using BuildColorsFunc =
            std::function<void(RendererModel const& model, VectorList<RenderColor>&)>;
        using BuildRenderModelsFunc = std::function<VectorList<RenderModel>(RendererModel& model)>;

    protected:
        using MeshType = PJ::Mesh;
        using ColorType = PJ::Color;

        bool meshNeedsBuild = true;
        bool vertexColorsNeedBuild = true;
        bool renderModelsNeedBuild = true;

        /// Renderer colors
        VectorList<ColorType> colors;

        /// Vertex colors (built from colors)
        VectorList<RenderColor> vertexColors;

        /// Mesh used for render
        MeshType mesh;
        Vector3 worldSize;

        /// Cached render model (updated when renderer changes)
        VectorList<RenderModel> renderModels;

        /// Builds mesh on demand
        BuildMeshFunc buildMeshFunc;

        /// Builds vertex colors on demand
        BuildColorsFunc buildVertexColorsFunc;

        /// Builds render models on demand
        BuildRenderModelsFunc buildRenderModelsFunc;

        /// Called when the model colors change
        RendererFunc onColorsChangeFunc;

    public:
        /// (Optional). Specifies z layer group for render
        int zIndex = 0;

        /// (Optional). Material for render
        SP<RenderMaterial> material;

        RendererModel(Vector3 worldSize);

        RenderMaterial* Material() const {
            return material.get();
        }

        VectorList<ColorType> Colors() const {
            return colors;
        }

        VectorList<ColorType>& Colors() {
            return colors;
        }

        void SetBuildMeshFunc(BuildMeshFunc value);
        void SetOnColorsChangeFunc(RendererFunc value);

        BuildColorsFunc BuildVertexColorsFunc() const {
            return buildVertexColorsFunc;
        }

        void SetBuildVertexColorsFunc(BuildColorsFunc value);

        void SetBuildRenderModelsFunc(BuildRenderModelsFunc value) {
            buildRenderModelsFunc = value;
            renderModelsNeedBuild = true;
        }

        // TODO: should this return const&?
        VectorList<RenderModel>& RenderModels() {
            if (renderModelsNeedBuild) {
                renderModelsNeedBuild = false;

                BuildIfNeeded();
                if (buildRenderModelsFunc) {
                    renderModels = buildRenderModelsFunc(*this);
                }
            }

            return renderModels;
        }

        void BuildIfNeeded() {
            if (meshNeedsBuild && buildMeshFunc) {
                meshNeedsBuild = false;
                mesh = buildMeshFunc(*this);
            }

            if (vertexColorsNeedBuild && buildVertexColorsFunc) {
                vertexColorsNeedBuild = false;
                buildVertexColorsFunc(*this, vertexColors);
            }
        }

        void SetColor(ColorType value);
        void SetAlpha(float value);

        ColorType Color() {
            GUARDR(!IsEmpty(colors), Color::white)
            return colors[0];
        }

        // FUTURE: introduce a dependency pattern here for these flags
        void SetMeshNeedsBuild() {
            SetRenderModelsNeedBuild();
            meshNeedsBuild = true;
            vertexColorsNeedBuild = true;
            renderModelsNeedBuild = true;
        }

        void SetVertexColorsNeedsBuild() {
            vertexColorsNeedBuild = true;
            renderModelsNeedBuild = true;
        }

        void SetRenderModelsNeedBuild() {
            renderModelsNeedBuild = true;
        }

        void SetColors(VectorList<ColorType> const& value);

        VectorList<RenderColor> const& VertexColors() {
            BuildIfNeeded();
            return vertexColors;
        }

        MeshType const& Mesh() {
            BuildIfNeeded();
            return mesh;
        }

        Vector3 WorldSize() const {
            return worldSize;
        }

        void SetWorldSize(Vector3 value);
    };
} // namespace PJ
