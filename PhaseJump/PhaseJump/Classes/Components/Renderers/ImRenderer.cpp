#include "ImRenderer.h"
#include "ColorRenderer.h"
#include "SpriteRenderer.h"
#include "TextRenderer.h"
#include "World.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

using This = ImRenderer;

ImRenderer::ImRenderer(Config config) :
    Base(config.worldSize),
    areShapesOpaque(config.areShapesOpaque) {
    Configure();
}

void ImRenderer::Add(ImPath& path) {
    paths.push_back(path);

    model.SetRenderModelsNeedBuild();
}

void ImRenderer::AddPath(PathConfig config) {
    ImPath path;
    path.renderType = config.renderType;

    path.items.push_back(config.item);
    Add(path);
}

This& ImRenderer::Image(String id, Vector2 origin) {
    return TemplateImage(id, origin, Color::white);
}

This& ImRenderer::TemplateImage(String id, Vector2 origin, std::optional<Color> color) {
    ImPathItem item;
    item.type = ImPathItemType::Image;

    Color itemColor = color ? *color : this->color;
    item.colors = { itemColor };
    item.frame.origin = origin;

    item.id = id;
    AddPath({ .item = item });

    return *this;
}

This& ImRenderer::FillRect(Rect rect, std::optional<Color> color) {
    ImPathItem item;
    item.type = ImPathItemType::Rect;

    Color itemColor = color ? *color : this->color;
    item.colors = { itemColor };

    item.frame = rect;
    AddPath({ .item = item });

    return *this;
}

This& ImRenderer::Text(StringView text, Vector2 pos, float fontSize, std::optional<Color> color) {
    ImPathItem item;
    item.type = ImPathItemType::Text;

    item.text = text;
    item.fontSpec.size = fontSize;
    Color itemColor = color ? *color : this->color;
    item.colors = { itemColor };

    item.frame.origin = pos;
    item.frame.size = { FloatMath::maxValue, FloatMath::maxValue };
    AddPath({ .item = item });

    return *this;
}

This& ImRenderer::Text(StringView text, Rect rect, float fontSize, std::optional<Color> color) {
    ImPathItem item;
    item.type = ImPathItemType::Text;

    item.text = text;
    item.fontSpec.size = fontSize;
    Color itemColor = color ? *color : this->color;
    item.colors = { itemColor };

    item.frame = rect;
    AddPath({ .item = item });

    return *this;
}

ImRenderer::TranslateItemFunc ImRenderer::MakeTranslateItemFunc(float down) {
    return [this, down](auto& renderer, auto& item) {
        auto worldSize = WorldSize();

        Matrix4x4 matrix;
        auto itemOrigin = item.frame.origin;
        auto itemSize = item.frame.size;
        float translateX =
            worldSize.x / 2.0f * vecLeft + itemSize.x / 2.0f * vecRight + itemOrigin.x * vecRight;
        float translateY =
            worldSize.y / 2.0f * -down + itemSize.y / 2.0f * down + itemOrigin.y * down;
        matrix.LoadTranslate({ translateX, translateY, 0 });

        return matrix;
    };
}

void ImRenderer::Configure() {
    String id;

    id = String(ImPathItemType::Rect) + String(".") + String(ImPathRenderType::Fill);
    buildRendererFuncs[id] = [this](auto& item) {
        UP<SomeRenderer> result = NEW<ColorRenderer>(ColorRenderer::Config{
            .color = item.GetColor(0), .worldSize = item.frame.size });

        // All immediate paths use blend so they batch render together
        // OPTIMIZE: if you know all shapes are opaque and will be behind all transparent textures,
        // you can optimize renders by using opaque render for shapes (Careful: this will result in
        // incorrect Z-order in some cases)
        static_cast<ColorRenderer*>(result.get())->EnableBlend(!areShapesOpaque);

        return result;
    };

    id = String(ImPathItemType::Text) + String(".") + String(ImPathRenderType::Fill);
    buildRendererFuncs[id] = [this](auto& item) -> UP<SomeRenderer> {
        GUARDR(owner, {})

        auto world = owner->World();
        GUARDR(world, {})

        SP<Font> font = world->FindFont(item.fontSpec);
        GUARDR(font, {})

        UP<SomeRenderer> result = NEW<TextRenderer>(TextRenderer::Config{
            .font = font, .text = item.text, .worldSize = item.frame.size });
        result->SetColor(item.GetColor(0, Color::white));
        return result;
    };

    id = String(ImPathItemType::Image) + String(".") + String(ImPathRenderType::Fill);
    buildRendererFuncs[id] = [this](auto& item) -> UP<SomeRenderer> {
        GUARDR(owner, {})

        auto world = owner->World();
        GUARDR(world, {})

        SP<SomeTexture> texture = world->FindTexture(item.id);
        GUARDR(texture, {})

        UP<SomeRenderer> result = NEW<SpriteRenderer>(SpriteRenderer::Config{ .texture = texture });
        result->SetColor(item.GetColor(0, Color::white));

        return result;
    };

    translateItemFunc = MakeTranslateItemFunc(vecDown);

    model.SetBuildRenderModelsFunc([this](auto& model) {
        VectorList<RenderModel> result;

        renderers.clear();

        for (auto& path : paths) {
            GUARD_CONTINUE(!IsEmpty(path.items))

            // FUTURE: support combining items
            auto item = path.items[0];

            try {
                String id = item.type + String(".") + path.renderType;

                auto buildRendererFunc = buildRendererFuncs.at(id);
                GUARD_CONTINUE(buildRendererFunc)

                UP<SomeRenderer> renderer = buildRendererFunc(item);
                GUARD_CONTINUE(renderer)

                auto renderModels = renderer->RenderModels();

                // If the item has no specified frame size, use the renderer size
                if (item.frame.size == Vector2()) {
                    item.frame.size = renderer->WorldSize();
                }

                // Give the renderer a chance to calculate its preferred size
                item.frame.size = renderer->CalculateSize(item.frame.size);

                Matrix4x4 matrix;
                if (translateItemFunc) {
                    matrix = translateItemFunc(*this, item);
                }

                for (auto& renderModel : renderModels) {
                    renderModel.mesh *= matrix;
                }
                AddRange(result, renderModels);

                // Keep the renderer and material in memory for the render
                renderers.push_back(std::move(renderer));
            } catch (...) {}

            if (item.type == ImPathItemType::Rect) {}
        }

        paths.clear();
        model.SetRenderModelsNeedBuild();

        return result;
    });
}
