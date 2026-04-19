#include "Polygon.h"
#include <cmath>
#include <limits>
#include <utility>

using namespace std;
using namespace PJ;

using This = Polygon;

Vector2 Polygon::CalculateMin() const {
    GUARDR(!IsEmpty(value), {})

    auto result = value[0];

    for (auto& vertex : value) {
        result = Vector2(std::min(result.x, vertex.x), std::min(result.y, vertex.y));
    }

    return result;
}

Vector2 Polygon::CalculateMax() const {
    GUARDR(!IsEmpty(value), {})

    auto result = value[0];

    for (auto& vertex : value) {
        result = { std::max(result.x, vertex.x), std::max(result.y, vertex.y) };
    }

    return result;
}

/**
 Reference:
 http://stackoverflow.com/questions/217578/point-in-polygon-aka-hit-test
 */
bool Polygon::TestHit(Vector2 pt) const {
    float testx = pt.x;
    float testy = pt.y;
    auto nvert = value.size();

    int c = 0;

    for (size_t i = 0, j = nvert - 1; i < nvert; j = i++) {
        if (((value[i].y > testy) != (value[j].y > testy)) &&
            (testx < (value[j].x - value[i].x) * (testy - value[i].y) / (value[j].y - value[i].y) +
                         value[i].x)) {
            c = !c;
        }
    }

    return c != 0;
}

String Polygon::ToString() const {
    stringstream s;
    s << "[";
    for (auto& v : value) {
        s << v.ToString() << ", ";
    }
    s << "]";

    return s.str();
}

This& Polygon::SetSize(Vector2 value) {
    // Once a polygon is zero size, it can never be resized
    GUARDR(value.x > 0 && value.y > 0, *this)

    auto center = Center();
    auto size = Size();
    GUARDR(size != value, *this);

    // Avoid divide by zero
    GUARDR(size.x > 0 && size.y > 0, *this)

    auto factor = value / size;

    for (auto& v : this->value) {
        auto distance = v - center;
        distance *= factor;
        v = center + distance;
    }

    SetBoundsInvalid();

    return *this;
}

This& Polygon::Offset(Vector2 value) {
    for (auto& v : this->value) {
        v += value;
    }
    SetBoundsInvalid();

    return *this;
}

namespace {
    /// Internal triangle representation used during Bowyer-Watson. Stores
    /// indices into a shared points array plus a cached circumcircle.
    struct BWTriangle {
        int a;
        int b;
        int c;
        Vector2 circumcenter;
        float circumradiusSq;
        bool valid = true;
    };

    /// Fills in `circumcenter` and `circumradiusSq` for the given triangle.
    /// Returns false if the points are colinear (degenerate).
    bool
    ComputeCircumcircle(Vector2 a, Vector2 b, Vector2 c, Vector2& outCenter, float& outRadiusSq) {
        float const d = 2.0f * (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y));
        if (std::abs(d) < 1e-12f) {
            return false;
        }

        float const aSq = a.x * a.x + a.y * a.y;
        float const bSq = b.x * b.x + b.y * b.y;
        float const cSq = c.x * c.x + c.y * c.y;

        float const ux = (aSq * (b.y - c.y) + bSq * (c.y - a.y) + cSq * (a.y - b.y)) / d;
        float const uy = (aSq * (c.x - b.x) + bSq * (a.x - c.x) + cSq * (b.x - a.x)) / d;

        outCenter = { ux, uy };

        float const dx = a.x - ux;
        float const dy = a.y - uy;
        outRadiusSq = dx * dx + dy * dy;
        return true;
    }

    /// Makes an undirected edge key from two point indices.
    std::pair<int, int> MakeEdge(int i, int j) {
        return i < j ? std::make_pair(i, j) : std::make_pair(j, i);
    }
} // namespace

VectorList<Triangle> Polygon::BuildTriangles() const {
    VectorList<Triangle> result;
    GUARDR(value.size() >= 3, result)

    // Fast path for a single triangle
    if (value.size() == 3) {
        Triangle t;
        t.value[0] = value[0];
        t.value[1] = value[1];
        t.value[2] = value[2];
        result.push_back(t);
        return result;
    }

    // Build a points array. Indices 0..2 are the super-triangle, indices
    // 3..3+N-1 are the polygon vertices in order.
    VectorList<Vector2> points;
    points.reserve(value.size() + 3);
    points.push_back({});
    points.push_back({});
    points.push_back({});

    for (auto const& v : value) {
        points.push_back(v);
    }

    // Construct a super-triangle that fully encloses the polygon's AABB.
    auto const minPt = Min();
    auto const maxPt = Max();
    float const dx = maxPt.x - minPt.x;
    float const dy = maxPt.y - minPt.y;
    float const deltaMax = std::max(dx, dy);

    // Guard against a zero-size input
    GUARDR(deltaMax > 0.0f, result)

    float const midX = (minPt.x + maxPt.x) * 0.5f;
    float const midY = (minPt.y + maxPt.y) * 0.5f;
    float const pad = deltaMax * 20.0f;

    points[0] = { midX - pad, midY - deltaMax };
    points[1] = { midX, midY + pad };
    points[2] = { midX + pad, midY - deltaMax };

    VectorList<BWTriangle> triangles;
    triangles.reserve(value.size() * 2);

    {
        BWTriangle super;
        super.a = 0;
        super.b = 1;
        super.c = 2;
        if (!ComputeCircumcircle(
                points[super.a], points[super.b], points[super.c], super.circumcenter,
                super.circumradiusSq
            )) {
            return result;
        }
        triangles.push_back(super);
    }

    // Insert polygon vertices one at a time (Bowyer-Watson).
    int const pointCount = (int)points.size();
    for (int pi = 3; pi < pointCount; pi++) {
        Vector2 const p = points[pi];

        // Mark bad triangles (point lies inside their circumcircle).
        VectorList<std::pair<int, int>> boundaryEdges;

        for (auto& tri : triangles) {
            if (!tri.valid) {
                continue;
            }
            float const ddx = p.x - tri.circumcenter.x;
            float const ddy = p.y - tri.circumcenter.y;
            if (ddx * ddx + ddy * ddy < tri.circumradiusSq) {
                tri.valid = false;

                // Collect this triangle's three edges; shared edges will
                // be canceled below.
                boundaryEdges.push_back(MakeEdge(tri.a, tri.b));
                boundaryEdges.push_back(MakeEdge(tri.b, tri.c));
                boundaryEdges.push_back(MakeEdge(tri.c, tri.a));
            }
        }

        // Boundary of the hole = edges appearing exactly once. Any edge
        // appearing twice is shared between two bad triangles and must be
        // discarded.
        VectorList<std::pair<int, int>> uniqueEdges;
        uniqueEdges.reserve(boundaryEdges.size());

        for (size_t i = 0; i < boundaryEdges.size(); i++) {
            bool shared = false;
            for (size_t j = 0; j < boundaryEdges.size(); j++) {
                if (i != j && boundaryEdges[i] == boundaryEdges[j]) {
                    shared = true;
                    break;
                }
            }
            if (!shared) {
                uniqueEdges.push_back(boundaryEdges[i]);
            }
        }

        // Drop invalidated triangles in-place to keep the list compact.
        VectorList<BWTriangle> kept;
        kept.reserve(triangles.size());
        for (auto const& tri : triangles) {
            if (tri.valid) {
                kept.push_back(tri);
            }
        }
        triangles = std::move(kept);

        // Retriangulate the hole by connecting each boundary edge to p.
        for (auto const& edge : uniqueEdges) {
            BWTriangle tri;
            tri.a = edge.first;
            tri.b = edge.second;
            tri.c = pi;
            if (!ComputeCircumcircle(
                    points[tri.a], points[tri.b], points[tri.c], tri.circumcenter,
                    tri.circumradiusSq
                )) {
                // Colinear — skip, it would be a zero-area triangle.
                continue;
            }
            triangles.push_back(tri);
        }
    }

    // Strip triangles that still reference a super-triangle vertex, then
    // keep only triangles whose centroid is inside the original polygon.
    // The centroid test correctly discards exterior triangles for concave
    // shapes (where Bowyer-Watson of the vertex set can produce triangles
    // that lie outside the polygon boundary).
    for (auto const& tri : triangles) {
        if (tri.a < 3 || tri.b < 3 || tri.c < 3) {
            continue;
        }

        Vector2 const va = points[tri.a];
        Vector2 const vb = points[tri.b];
        Vector2 const vc = points[tri.c];
        Vector2 const centroid = {
            (va.x + vb.x + vc.x) / 3.0f,
            (va.y + vb.y + vc.y) / 3.0f,
        };

        if (!TestHit(centroid)) {
            continue;
        }

        Triangle t;
        t.value[0] = va;
        t.value[1] = vb;
        t.value[2] = vc;
        result.push_back(t);
    }

    return result;
}
