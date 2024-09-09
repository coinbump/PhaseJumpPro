//
//  QuadMeshBuilder.cpp
//  PhaseJump
//
//  Created by Jeremy Vineyard on 9/3/24.
//

#include "QuadMeshBuilder.h"
#include "VectorList.h"

using namespace std;
using namespace PJ;

// TODO: should vertices in quad be cartesian oriented? (bottom-left first)? Does it matter?
VectorList<uint32_t> const QuadMesh::triangles{ 0, 1, 2, 1, 3, 2 };

// It only makes sense to share these if all textures are the same
VectorList<Vector2> const QuadMesh::uvs{ Vector2(0, 0), Vector2(1, 0), Vector2(0, 1),
                                         Vector2(1, 1) };
