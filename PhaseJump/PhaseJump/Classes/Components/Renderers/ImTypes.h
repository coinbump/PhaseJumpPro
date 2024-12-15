#pragma once

#include "Angle.h"
#include "Color.h"
#include "Font.h"
#include "Polygon.h"
#include "Rect.h"
#include "RoundCornersMeshBuilder.h"
#include "Vector2.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: 12/7/24
 */
namespace PJ {
    namespace ImPathRenderType {
        /// Fill the path with a color
        auto constexpr Fill = "fill";

        /// Draw the outline of the path with a stroke
        auto constexpr Frame = "frame";
    } // namespace ImPathRenderType

    /// Immedate mode path item type IDs
    namespace ImPathItemType {
        auto constexpr Capsule = "capsule";
        auto constexpr MoveTo = "moveTo";
        auto constexpr LineTo = "lineTo";
        auto constexpr Rect = "rect";
        auto constexpr RoundCorners = "roundCorners";

        /// Used to render arcs, ellipses, and circles
        auto constexpr Arc = "arc";

        auto constexpr Polygon = "poly";
        auto constexpr Text = "text";
        auto constexpr Image = "image";
    } // namespace ImPathItemType

    /// Defines an item component of a complex path for immediate mode renders
    /// Some paths can be combined into another path (multiple lines -> polygon)
    class ImPathItem {
    public:
        String type;

        /// Unique identifier (Example: image id)
        String id;

        /// Required: frame for translation
        Rect frame;

        /// Shape for polygon paths
        Polygon poly;
        PolyClose polyClose{};

        VectorList<Vector2> vectors;
        VectorList<Color> colors;

        float strokeWidth = 1;

        PathCap startPathCap = PathCap::Flat;
        PathCap endPathCap = PathCap::Flat;
        PathCorner pathCorner = PathCorner::Round;

        String text;
        FontSpec fontSpec;

        /// Primary axis
        Axis2D axis{};

        /// Round corners
        RoundCorners roundCorners;

        /// First angle of this item (example: start angle for arc)
        Angle startAngle;

        /// Distance from start angle
        Angle angleDelta;

        Vector2 GetVector(size_t index, Vector2 defaultValue = {}) {
            return index >= 0 && index < vectors.size() ? vectors[index] : defaultValue;
        }

        Color GetColor(size_t index, Color defaultValue = {}) {
            return index >= 0 && index < colors.size() ? colors[index] : defaultValue;
        }
    };

    /// Defines a complete path for immediate mode renders
    /// Some paths can be combined into another path (multiple lines -> polygon)
    class ImPath {
    public:
        VectorList<ImPathItem> items;
        String renderType = ImPathRenderType::Fill;

        // FUTURE: add combine support if needed
        // void Combine()
    };
} // namespace PJ
