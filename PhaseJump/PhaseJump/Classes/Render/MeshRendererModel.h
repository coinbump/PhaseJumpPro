#pragma once

#include "SomeRenderer.h"
#include "WorldSizeable.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 8/11/24
 */
namespace PJ {
    /// Standard model for a renderer that renders a mesh
    class MeshRendererModel {
    protected:
        using MeshType = PJ::Mesh;
        using MeshBuilderFuncType = std::function<Mesh(MeshRendererModel const& model)>;

        /// Mesh used for render
        MeshType mesh;
        Vector3 worldSize;

        /// Mesh builder
        /// Can be replaced but the default behavior for the renderer might not match the new mesh
        /// shape
        MeshBuilderFuncType meshBuilderFunc;

    public:
        MeshRendererModel(Vector3 worldSize) :
            worldSize(worldSize) {}

        MeshBuilderFuncType MeshBuilderFunc() const {
            return meshBuilderFunc;
        }

        void SetMeshBuilderFunc(MeshBuilderFuncType meshBuilderFunc);

        MeshType& Mesh() {
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
