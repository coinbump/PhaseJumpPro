#pragma once

#include "Color.h"
#include "ImTypes.h"
#include "Renderer.h"
#include "RoundCornersMeshBuilder.h"
#include "SomeShape.h"
#include "StencilPushRenderModel.h"
#include "UnorderedMap.h"
#include "Vector2.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/16/24
 */
namespace PJ {
    class Matrix4x4;

    /**
     Immediate mode renderer

     Uses a Y-up Cartesian coordinate system with the origin at the bottom-left of its world

     Perform the immediate render by using the RenderPrepare signal
     */
    class ImRenderer : public Renderer {
    public:
        using Base = Renderer;
        using This = ImRenderer;

        using TranslateItemFunc = std::function<Matrix4x4(This&, ImPathItem&)>;
        using BuildRendererFunc = std::function<UP<Renderer>(ImPathItem&)>;
        using BuildRendererFuncsMap = UnorderedMap<String, BuildRendererFunc>;

        TranslateItemFunc translateItemFunc;

        /// Stores cached render models and rebuilds when needed
        class RendererModel {
        public:
            using This = RendererModel;

            using BuildRenderModelsFunc = std::function<RenderModelList(RendererModel& model)>;

        protected:
            bool renderModelsNeedBuild = true;

            /// Builds render models on demand
            BuildRenderModelsFunc buildRenderModelsFunc;

            /// Cached render model (updated when renderer changes)
            RenderModelList renderModels;

            Vector3 worldSize;

        public:
            RendererModel(Vector3 worldSize) :
                worldSize(worldSize) {}

            Vector3 WorldSize() const {
                return worldSize;
            }

            void SetWorldSize(Vector3 value) {
                GUARD(worldSize != value)
                worldSize = value;

                SetRenderModelsNeedBuild();
            }

            void SetBuildRenderModelsFunc(BuildRenderModelsFunc value) {
                buildRenderModelsFunc = value;
                renderModelsNeedBuild = true;
            }

            RenderModelList& RenderModels() {
                if (renderModelsNeedBuild) {
                    renderModelsNeedBuild = false;

                    if (buildRenderModelsFunc) {
                        renderModels = buildRenderModelsFunc(*this);
                    }
                }

                return renderModels;
            }

            void SetRenderModelsNeedBuild() {
                renderModelsNeedBuild = true;
            }
        };

        RendererModel model;

    protected:
        Color color = Color::black;
        Color strokeColor = Color::black;

        /// Paths built by immediate mode render commands
        VectorList<ImPath> paths;

        /// Keeps renderers and materials in memory for next render
        /// Required because we share data from the renderer with the render engine
        VectorList<UP<Renderer>> renderers;

        /// Maps "itemType.renderType" to a build renderer func
        UnorderedMap<String, BuildRendererFunc> buildRendererFuncs;

        /// Material used to draw into the stencil buffer. Color doesn't matter
        SP<RenderMaterial> stencilMaterial;

        String RendererId(String itemType, String pathRenderType);

        PathCap startPathCap = PathCap::Flat;
        PathCap endPathCap = PathCap::Flat;
        PathCorner pathCorner = PathCorner::Round;
        float strokeWidth = 1;

        /// Optimize: set to true to draw images without blending, which can result in
        /// faster renders, but affects the draw order (non-blended objects are drawn behind blended
        /// objects)
        bool areImagesOpaque{};

        /// If true, the renderer will continue to render whatever was last rendered
        /// Optimize: Call Freeze(true) after rendering static content. This improves render
        /// performance
        bool isFrozen{};

    public:
        /// Optimize: shapes are drawn with blend mode on by default, so they have proper Z ordering
        /// when drawn with transparent textures If you know that all shapes will be rendered behind
        /// all transparent textures, you can optimize your renders by setting this flag to true
        bool areShapesOpaque{};

        struct Config {
            Vector2 worldSize;
            bool areShapesOpaque{};
        };

        ImRenderer(Config const& config);

        // Fix for Clang compile bug:
        // https://stackoverflow.com/questions/64138924/struct-with-default-members-initializers-cannot-be-inside-class-and-used-as-defa
        ImRenderer() :
            ImRenderer(Config{}) {}

        void Freeze(bool value) {
            isFrozen = value;
        }

        /// Sets the opaque setting for any subsequent images added
        void SetAreImagesOpaque(bool value) {
            areImagesOpaque = value;
        }

        /// Sets the color to be used by the next render option, if a color is not provided
        This& SetForegroundColor(Color value) {
            color = value;
            return *this;
        }

        /// Sets the stroke color for framed shapes
        This& SetStrokeColor(Color value) {
            strokeColor = value;
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

        /// Draws an image texture
        This& Image(SP<Texture> texture, Vector2 origin);

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

        This& PolyLine(Polygon poly, std::optional<Color> color = {}) {
            return FramePolygon(poly, PolyClose::Open, color);
        }

        /// Frame a bezier path with the color, or use the default color if not specified.
        /// `segmentDistance` controls the polyline resolution — smaller values produce
        /// a smoother curve at higher mesh cost.
        This& FrameBezier(
            BezierPath bezierPath, float segmentDistance = 10, std::optional<Color> color = {}
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

        /// Push a stencil clip. Call ClipPop to pop
        This& ClipPush(Rect frame, UP<SomeShape> shape);

        /// Push a rectangular stencil clip scope. Convenience for `ClipPush(frame, RectShape)`
        This& ClipPush(Rect frame);

        /// Pop the most recently pushed stencil clip. Has no effect if there is nothing to pop
        This& ClipPop();

        /// Use to customize item translation for different coordinate systems
        TranslateItemFunc MakeTranslateItemFunc(float down = vecDown);

        // MARK: Renderer

        void SetColor(Color color) override {
            SetForegroundColor(color);
        }

        RenderModelList RenderModels() override {
            auto& cachedModels = model.RenderModels();

            auto matrix = ModelMatrix();
            RenderModelList result;
            result.reserve(cachedModels.size());
            for (auto& sp : cachedModels) {
                if (auto* material = dynamic_cast<MaterialRenderModel*>(sp.get())) {
                    material->matrix = matrix;
                } else if (auto* push = dynamic_cast<StencilPushRenderModel*>(sp.get())) {
                    push->matrix = matrix;
                }
                result.push_back(sp);
            }

            return result;
        }

        // MARK: WorldSizeable

        Vector3 WorldSize() const override {
            return model.WorldSize();
        }

        void SetWorldSize(Vector3 value) override {
            model.SetWorldSize(value);
        }

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "ImRenderer";
        }

    protected:
        struct PathConfig {
            ImPathItem item;
            String renderType = ImPathRenderType::Fill;
        };

        void AddPath(PathConfig config);

        void Configure();

        void Add(ImPath path);
    };
} // namespace PJ
