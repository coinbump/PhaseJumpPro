#pragma once

#include "Color.h"
#include "ImTypes.h"
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

        void SetColor(Color value) {
            color = value;
        }

        /// Draws an image
        This& Image(String id, Vector2 origin);

        /// Draws a template image, which is modulated by a color
        This& TemplateImage(String id, Vector2 origin, std::optional<Color> color = {});

        /// Fill a rectangle with the color, or use the default color if not specified
        This& FillRect(Rect rect, std::optional<Color> color = {});

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
