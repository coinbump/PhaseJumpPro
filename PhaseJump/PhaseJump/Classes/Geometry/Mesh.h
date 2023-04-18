#ifndef PJMESH_H
#define PJMESH_H

#include "VectorList.h"
#include "Vector3.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 4/14/23
 */
namespace PJ {
    /// Stores vertices of a mesh
    class Mesh : public VectorList<Vector3> {
    public:
        Mesh(VectorList<Vector3> const& rhs) : VectorList<Vector3>(rhs) {
        }
    };
}

#endif
