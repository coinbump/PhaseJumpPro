#ifndef PJVERTEXLIST_H
#define PJVERTEXLIST_H

#include "Vector3.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 9/1/24
 */
namespace PJ {
    /// Stores vertices of a mesh
    using VertexList = VectorList<Vector3>;
} // namespace PJ

#endif
