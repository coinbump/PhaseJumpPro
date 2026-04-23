#pragma once

#include "Angle.h"
#include "BezierPath.h"
#include "Color.h"
#include "Font.h"
#include "Polygon.h"
#include "Rect.h"
#include "RoundCornersMeshBuilder.h"
#include "SomeShape.h"
#include "Vector2.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: 12/7/24
 */
namespace PJ {
    class Texture;
    class Font;

    namespace ImPathRenderType {
        /// Fill the path with a color
        auto constexpr Fill = "fill";

        /// Draw the outline of the path with a stroke
        auto constexpr Frame = "frame";
    } // namespace ImPathRenderType

    /// Immedate mode path item type IDs
    namespace ImPathItemType {
        auto constexpr Capsule = "capsule";
        auto constexpr Rect = "rect";
        auto constexpr RoundCorners = "roundCorners";

        /// Used to render arcs, ellipses, and circles
        auto constexpr Arc = "arc";

        auto constexpr Polygon = "poly";
        auto constexpr Bezier = "bezier";
        auto constexpr Text = "text";
        auto constexpr Image = "image";

        /// Pushes a stencil clip using the item's clipShape
        auto constexpr ClipPush = "clip.push";

        /// Pops the most recent stencil clip
        auto constexpr ClipPop = "clip.pop";

        // FUTURE: support context translate and rotation:
        // https://stackoverflow.com/questions/8774001/core-graphics-rotate-rectangle

        // FUTURE: support these if needed
        auto constexpr MoveTo = "moveTo";
        auto constexpr LineTo = "lineTo";
        auto constexpr TurtleForward = "turtle.forward";
        auto constexpr TurtleLeft = "turtle.left";
        auto constexpr TurtleRight = "turtle.right";
        auto constexpr TurtlePenUp = "turtle.penUp";
        auto constexpr TurtlePenDown = "turtle.penDown";
    } // namespace ImPathItemType

    /// Defines an item component of a path-type object for immediate mode renders
    class ImPathItem {
    public:
        using This = ImPathItem;

        String type;

        /// Unique identifier (Example: image id)
        String id;

        /// Required: frame for translation
        Rect frame;

        /// Shape for polygon paths
        Polygon poly;
        PolyClose polyClose{};

        /// Shape for bezier paths
        BezierPath bezierPath;

        /// Target world-space distance between sampled points for bezier paths
        float segmentDistance = 10;

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

        /// If true, always render the item without transparency
        bool isOpaque{};

        /// Image texture
        SP<Texture> texture{};

        /// Text font
        SP<Font> font{};

        /// Clip shape for ClipPush items. The shape's mesh defines the stencil region
        UP<SomeShape> clipShape;

        ImPathItem() = default;
        ImPathItem(This&&) noexcept = default;
        This& operator=(This&&) noexcept = default;

        ImPathItem(This const& other) :
            type(other.type),
            id(other.id),
            frame(other.frame),
            poly(other.poly),
            polyClose(other.polyClose),
            bezierPath(other.bezierPath),
            segmentDistance(other.segmentDistance),
            vectors(other.vectors),
            colors(other.colors),
            strokeWidth(other.strokeWidth),
            startPathCap(other.startPathCap),
            endPathCap(other.endPathCap),
            pathCorner(other.pathCorner),
            text(other.text),
            fontSpec(other.fontSpec),
            axis(other.axis),
            roundCorners(other.roundCorners),
            startAngle(other.startAngle),
            angleDelta(other.angleDelta),
            isOpaque(other.isOpaque),
            texture(other.texture),
            font(other.font),
            clipShape(other.clipShape ? other.clipShape->Clone() : nullptr) {}

        This& operator=(This const& other) {
            if (this == &other) {
                return *this;
            }
            type = other.type;
            id = other.id;
            frame = other.frame;
            poly = other.poly;
            polyClose = other.polyClose;
            bezierPath = other.bezierPath;
            segmentDistance = other.segmentDistance;
            vectors = other.vectors;
            colors = other.colors;
            strokeWidth = other.strokeWidth;
            startPathCap = other.startPathCap;
            endPathCap = other.endPathCap;
            pathCorner = other.pathCorner;
            text = other.text;
            fontSpec = other.fontSpec;
            axis = other.axis;
            roundCorners = other.roundCorners;
            startAngle = other.startAngle;
            angleDelta = other.angleDelta;
            isOpaque = other.isOpaque;
            texture = other.texture;
            font = other.font;
            clipShape = other.clipShape ? other.clipShape->Clone() : nullptr;
            return *this;
        }

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
