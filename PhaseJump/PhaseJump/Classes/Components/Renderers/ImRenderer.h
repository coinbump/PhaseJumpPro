#pragma once

#include "Color.h"
#include "ImTypes.h"
#include "RoundCornersMeshBuilder.h"
#include "SomeRenderer.h"
#include "UnorderedMap.h"
#include "Vector2.h"
#include "VectorList.h"

/*
 RATING: 4 stars
 Tested and works, but limited
 CODE REVIEW: 12/7/24
 */
namespace PJ {
    class Matrix4x4;

    /**
     Immediate mode renderer

     As opposed to the traditional ECS model of creating persistent renderers, this pattern allows
     you to draw in immediate mode This can make building certain types of apps easier

     Perform the immediate render by using the RenderPrepare signal
     */
    class ImRenderer : public SomeRenderer {
    public:
        using Base = SomeRenderer;
        using This = ImRenderer;

        using TranslateItemFunc = std::function<Matrix4x4(This&, ImPathItem&)>;
        using BuildRendererFunc = std::function<UP<SomeRenderer>(ImPathItem&)>;
        using BuildRendererFuncsMap = UnorderedMap<String, BuildRendererFunc>;

        TranslateItemFunc translateItemFunc;

    protected:
        Color color = Color::black;

        // FUTURE: combine multiple shapes into 1 for faster renders
        /// Paths built by immediate mode render commands
        VectorList<ImPath> paths;

        /// Keeps renderers and materials in memory for next render
        VectorList<UP<SomeRenderer>> renderers;

        /// Maps "itemType.renderType" to a build renderer func
        UnorderedMap<String, BuildRendererFunc> buildRendererFuncs;

        String RendererId(String itemType, String pathRenderType);

        PathCap startPathCap = PathCap::Flat;
        PathCap endPathCap = PathCap::Flat;
        PathCorner pathCorner = PathCorner::Round;
        float strokeWidth = 1;

    public:
        /// OPTIMIZE: shapes are drawn with blend mode on by default, so they have proper Z ordering
        /// when drawn with transparent textures If you know that all shapes will be rendered behind
        /// all transparent textures, you can optimize your renders by setting this flag to true
        bool areShapesOpaque{};

        struct Config {
            Vector2 worldSize;
            bool areShapesOpaque{};
        };

        ImRenderer(Config config);

        // Fix for Clang compile bug:
        // https://stackoverflow.com/questions/64138924/struct-with-default-members-initializers-cannot-be-inside-class-and-used-as-defa
        ImRenderer() :
            ImRenderer(Config{}) {}

        This& SetColor(Color value) {
            color = value;
            return *this;
        }

        /// Sets the start cap type for paths
        This& SetStartPathCap(PathCap value) {
            startPathCap = value;
            return *this;
        }

        /// Sets the end cap type for paths
        This& SetEndPathCap(PathCap value) {
            endPathCap = value;
            return *this;
        }

        /// Sets the corner type for paths
        This& SetPathCorner(PathCorner value) {
            pathCorner = value;
            return *this;
        }

        /// Sets the stroke width for framed shapes
        This& SetStrokeWidth(float value) {
            strokeWidth = value;
            return *this;
        }

        /// Draws an image
        This& Image(String id, Vector2 origin);

        /// Draws a template image, which is modulated by a color
        This& TemplateImage(String id, Vector2 origin, std::optional<Color> color = {});

        /// Draw a line with the color, or use the default color if not specified
        This& Line(Vector2 start, Vector2 end, std::optional<Color> color = {});

        /// Frame a circle with the color, or use the default color if not specified
        This& FrameCircle(Vector2 origin, float radius, std::optional<Color> color = {});

        /// Fill a circle with the color, or use the default color if not specified
        This& FillCircle(Vector2 origin, float radius, std::optional<Color> color = {});

        /// Frame an arc with the color, or use the default color if not specified
        This&
        FrameArc(Rect frame, Angle startAngle, Angle angleDelta, std::optional<Color> color = {});

        /// Fill an arc with the color, or use the default color if not specified
        This&
        FillArc(Rect frame, Angle startAngle, Angle angleDelta, std::optional<Color> color = {});

        /// Frame an ellipse with the color, or use the default color if not specified
        This& FrameEllipse(Rect frame, std::optional<Color> color = {});

        /// Fill an ellipse with the color, or use the default color if not specified
        This& FillEllipse(Rect frame, std::optional<Color> color = {});

        /// Frame a polygon with the color, or use the default color if not specified
        This& FramePolygon(
            Polygon poly, PolyClose polyClose = PolyClose::Open, std::optional<Color> color = {}
        );

        /// Frame a rectangle with the color, or use the default color if not specified
        This& FrameRect(Rect frame, std::optional<Color> color = {});

        /// Fill a rectangle with the color, or use the default color if not specified
        This& FillRect(Rect frame, std::optional<Color> color = {});

        /// Fill a capsule with the color, or use the default color if not specified
        This& FillCapsule(Rect frame, Axis2D axis = Axis2D::X, std::optional<Color> color = {});

        /// Fill a rounded rectangle with the color, or use the default color if not specified
        This& FillRoundRect(Rect frame, float cornerRadius = 16, std::optional<Color> color = {});

        /// Fill a rounded corners shape with the color, or use the default color if not specified
        This& FillRoundCorners(
            Rect frame, RoundCorners corners = RoundCorners::WithRadius(16),
            std::optional<Color> color = {}
        );

        /// Draw unwrapped text at the specified position
        This& Text(StringView text, Vector2 pos, float fontSize, std::optional<Color> color = {});

        /// Draw wrapped text within the specified frame
        This& Text(StringView text, Rect rect, float fontSize, std::optional<Color> color = {});

        /// Use to customize item translation for different coordinate systems
        TranslateItemFunc MakeTranslateItemFunc(float down = vecDown);

    protected:
        struct PathConfig {
            ImPathItem item;
            String renderType = ImPathRenderType::Fill;
        };

        void AddPath(PathConfig config);

        void Configure();

        void Add(ImPath& path);
    };
} // namespace PJ
