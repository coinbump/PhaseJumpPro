#pragma once

#include "Polygon.h"
#include "SomeMeshBuilder.h"

namespace PJ {
    enum class PolyClose { Open, Closed };

    /**
     Builds a mesh that shows the frame of a polygon

     NOTE: this is a basic implementation with no support for high quality features like correct
     miters FUTURE: support correct miter, rounded corners, etc.
     */
    class PolyFrameMeshBuilder : SomeMeshBuilder {
    public:
        float strokeWidth = 2;

        /// Determines how to render the final segment (from final vertex to last vertex)
        PolyClose polyClose = PolyClose::Closed;

        Polygon poly;

        PolyFrameMeshBuilder(
            Polygon poly, float strokeWidth, PolyClose polyClose = PolyClose::Closed
        ) :
            poly(poly),
            polyClose(polyClose),
            strokeWidth(strokeWidth) {}

        size_t PolyVertexCount() const {
            return poly.Count();
        }

        size_t SegmentCount() const {
            switch (polyClose) {
            case PolyClose::Closed:
                return PolyVertexCount();
                break;
            case PolyClose::Open:
                return PolyVertexCount() - 1;
                break;
            }
        }

        size_t MeshVertexCount() const {
            // Each segment of a poly must be its own quad, in order for the stroke width to
            // Render at the proper offset along that segment.
            return SegmentCount() * 4;
        }

        Mesh BuildMesh() override;
    };
} // namespace PJ
