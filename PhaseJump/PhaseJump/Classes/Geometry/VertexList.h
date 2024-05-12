#ifndef PJVERTEXLIST_H
#define PJVERTEXLIST_H

#include "VectorList.h"
#include "Vector3.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 4/14/24
 */
namespace PJ {
    /// Stores vertices of a mesh
    using VertexList = VectorList<Vector3>;
}

#endif
