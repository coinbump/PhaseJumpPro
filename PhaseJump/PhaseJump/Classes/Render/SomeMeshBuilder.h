#ifndef PJSOMEMESHBUILDER_H
#define PJSOMEMESHBUILDER_H

#include "WorldComponent.h"
#include "Mesh.h"
#include "VertexList.h"

/*
 RATING: 5 stars
 Simple interface
 CODE REVIEW: 4/14/23
 */
namespace PJ {
    /// Builds a render mesh with vertices
    class SomeMeshBuilder : public WorldComponent {
    public:
        using Base = WorldComponent;

        virtual Mesh BuildMesh() = 0;

        VertexList BuildVertexList();

        void Awake() override;
    };
}

#endif
