#ifndef PJSOMERENDERMESHBUILDER_H
#define PJSOMERENDERMESHBUILDER_H

#include "WorldComponent.h"

/*
 RATING: 5 stars
 Simple interface
 CODE REVIEW: 4/14/23
 */
namespace PJ {
    class RenderMesh;
    class Mesh;

    /// Builds a render mesh with vertices
    class SomeRenderMeshBuilder : public WorldComponent {
    public:
        using Base = WorldComponent;

        virtual RenderMesh BuildRenderMesh() = 0;

        Mesh BuildMesh();

        void Awake() override;
    };
}

#endif
