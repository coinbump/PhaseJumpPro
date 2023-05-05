#ifndef PJSIMPLEPOLYGONCOLLIDER2D_H
#define PJSIMPLEPOLYGONCOLLIDER2D_H

#include "SomeCollider2D.h"
#include "Polygon.h"

namespace PJ {
    /// Simple 2D polygon collider
    class SimplePolygonCollider2D : public SomeCollider2D {
    public:
        Polygon polygon;
    };
}

#endif
