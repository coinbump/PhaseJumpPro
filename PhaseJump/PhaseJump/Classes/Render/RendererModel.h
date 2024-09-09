#pragma once

#include "Mesh.h"
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
        using MeshBuilderFuncType = std::function<Mesh(RendererModel const& model)>;

    protected:
        using MeshType = PJ::Mesh;

        bool meshNeedsBuild = true;

        /// Mesh used for render
        MeshType mesh;
        Vector3 worldSize;

        /// (Optional). Builds mesh on demand
        MeshBuilderFuncType meshBuilderFunc;

    public:
        /// (Optional). Allows render model to pass render hints to render processors
        Tags tags;

        /// (Optional). Specifies z layer group for render
        int zIndex = 0;

        /// (Optional). Material for render
        SP<RenderMaterial> material;

        RendererModel(Vector3 worldSize) :
            worldSize(worldSize) {}

        MeshBuilderFuncType MeshBuilderFunc() const {
            return meshBuilderFunc;
        }

        void SetMeshBuilderFunc(MeshBuilderFuncType value);

        void BuildIfNeeded() {
            GUARD(meshNeedsBuild && meshBuilderFunc)
            meshNeedsBuild = false;
            mesh = meshBuilderFunc(*this);
        }

        MeshType& Mesh() {
            BuildIfNeeded();
            return mesh;
        }

        MeshType const& Mesh() const {
            return mesh;
        }

        Vector3 WorldSize() const {
            return worldSize;
        }

        void SetWorldSize(Vector3 value);
    };
} // namespace PJ
