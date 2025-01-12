#include "ImRenderer.h"
#include "CapsuleMeshBuilder.h"
#include "CenterPolyFrameMeshBuilder.h"
#include "ColorRenderer.h"
#include "EllipseMeshBuilder.h"
#include "PolyFrameMeshBuilder.h"
#include "QuadFrameMeshBuilder.h"
#include "SpriteRenderer.h"
#include "TextRenderer.h"
#include "World.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

using This = ImRenderer;

ImRenderer::ImRenderer(Config config) :
    model(config.worldSize),
    areShapesOpaque(config.areShapesOpaque) {
    Configure();
}

void ImRenderer::Add(ImPath& path) {
    paths.push_back(path);

    model.SetRenderModelsNeedBuild();
}

void ImRenderer::AddPath(PathConfig config) {
    GUARD(!isFrozen)

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
    item.isOpaque = areImagesOpaque;

    item.id = id;
    AddPath({ .item = item });

    return *this;
}

This& ImRenderer::Image(SP<SomeTexture> texture, Vector2 origin) {
    GUARDR(texture, *this)

    ImPathItem item;
    item.type = ImPathItemType::Image;

    Color itemColor = Color::white;
    item.colors = { itemColor };
    item.frame.origin = origin;
    item.isOpaque = areImagesOpaque;

    item.texture = texture;
    AddPath({ .item = item });

    return *this;
}

This& ImRenderer::FramePolygon(Polygon poly, PolyClose polyClose, std::optional<Color> color) {
    ImPathItem item;
    item.type = ImPathItemType::Polygon;

    Color itemColor = color ? *color : this->strokeColor;
    item.colors = { itemColor };

    item.frame.origin = poly.Min();
    item.frame.size = poly.Size();

    auto polyCenter = poly.Center();
    auto polyOffset = -polyCenter;
    poly.Offset(polyOffset);

    item.poly = poly;
    item.strokeWidth = strokeWidth;
    item.startPathCap = startPathCap;
    item.endPathCap = endPathCap;
    item.pathCorner = pathCorner;
    item.polyClose = polyClose;
    AddPath({ .item = item, .renderType = ImPathRenderType::Frame });

    return *this;
}

This& ImRenderer::Line(Vector2 start, Vector2 end, std::optional<Color> color) {
    return FramePolygon({ start, end }, PolyClose::Open, color);
}

This& ImRenderer::FrameRect(Rect frame, std::optional<Color> color) {
    ImPathItem item;
    item.type = ImPathItemType::Rect;

    Color itemColor = color ? *color : this->strokeColor;
    item.colors = { itemColor };

    item.frame = frame;
    item.strokeWidth = strokeWidth;
    AddPath({ .item = item, .renderType = ImPathRenderType::Frame });

    return *this;
}

This& ImRenderer::FillRect(Rect frame, std::optional<Color> color) {
    ImPathItem item;
    item.type = ImPathItemType::Rect;

    Color itemColor = color ? *color : this->color;
    item.colors = { itemColor };

    item.frame = frame;
    AddPath({ .item = item });

    return *this;
}

This& ImRenderer::FillCapsule(Rect frame, Axis2D axis, std::optional<Color> color) {
    ImPathItem item;
    item.type = ImPathItemType::Capsule;

    Color itemColor = color ? *color : this->color;
    item.colors = { itemColor };
    item.axis = axis;

    item.frame = frame;
    AddPath({ .item = item });

    return *this;
}

This& ImRenderer::FillRoundRect(Rect frame, float cornerRadius, std::optional<Color> color) {
    return FillRoundCorners(frame, RoundCorners::WithRadius(cornerRadius), color);
}

This& ImRenderer::FillRoundCorners(Rect frame, RoundCorners corners, std::optional<Color> color) {
    ImPathItem item;
    item.type = ImPathItemType::RoundCorners;

    Color itemColor = color ? *color : this->color;
    item.colors = { itemColor };
    item.roundCorners = corners;

    item.frame = frame;
    AddPath({ .item = item });

    return *this;
}

This& ImRenderer::FrameCircle(Vector2 origin, float radius, std::optional<Color> color) {
    Rect frame{ { origin.x - radius, origin.y - radius }, { radius * 2.0f, radius * 2.0f } };
    return FrameEllipse(frame, color);
}

This& ImRenderer::FillCircle(Vector2 origin, float radius, std::optional<Color> color) {
    Rect frame{ { origin.x - radius, origin.y - radius }, { radius * 2.0f, radius * 2.0f } };
    return FillEllipse(frame, color);
}

This& ImRenderer::FrameEllipse(Rect frame, std::optional<Color> color) {
    return FrameArc(frame, {}, Angle::WithDegrees(360), color);
}

This& ImRenderer::FillEllipse(Rect frame, std::optional<Color> color) {
    return FillArc(frame, {}, Angle::WithDegrees(360), color);
}

This& ImRenderer::FrameArc(
    Rect frame, Angle startAngle, Angle angleDelta, std::optional<Color> color
) {
    ImPathItem item;
    item.type = ImPathItemType::Arc;

    Color itemColor = color ? *color : this->strokeColor;
    item.colors = { itemColor };

    item.frame = frame;
    item.startAngle = startAngle;
    item.angleDelta = angleDelta;
    item.strokeWidth = strokeWidth;
    AddPath({ .item = item, .renderType = ImPathRenderType::Frame });

    return *this;
}

This& ImRenderer::FillArc(
    Rect frame, Angle startAngle, Angle angleDelta, std::optional<Color> color
) {
    ImPathItem item;
    item.type = ImPathItemType::Arc;

    Color itemColor = color ? *color : this->color;
    item.colors = { itemColor };

    item.frame = frame;
    item.startAngle = startAngle;
    item.angleDelta = angleDelta;
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

    GUARDR(owner, *this)

    auto world = owner->World();
    GUARDR(world, *this)

    item.font = FindFont(world->resources, item.fontSpec);
    GUARDR(item.font, *this)

    // Calculate the text size
    TextMeasurer tm(*item.font);
    auto metrics =
        tm.Measure(item.text, { FloatMath::maxValue, FloatMath::maxValue }, LineClip::None);
    item.frame.size = metrics.CalculateSize();

    AddPath({ .item = item });

    return *this;
}

This& ImRenderer::Text(StringView text, Rect rect, float fontSize, std::optional<Color> color) {
    ImPathItem item;
    item.type = ImPathItemType::Text;

    item.text = text;
    item.fontSpec.size = fontSize;

    GUARDR(owner, *this)

    auto world = owner->World();
    GUARDR(world, *this)

    item.font = FindFont(world->resources, item.fontSpec);
    GUARDR(item.font, *this)

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
            worldSize.y / 2.0f * down + itemSize.y / 2.0f * -down + itemOrigin.y * -down;
        matrix.LoadTranslate({ translateX, translateY, 0 });

        return matrix;
    };
}

String ImRenderer::RendererId(String itemType, String pathRenderType) {
    return itemType + String(".") + pathRenderType;
}

void ImRenderer::Configure() {
    String id;

    // MARK: Rect

    id = RendererId(ImPathItemType::Rect, ImPathRenderType::Fill);
    buildRendererFuncs[id] = [this](auto& item) {
        UP<SomeRenderer> result = NEW<ColorRenderer>(ColorRenderer::Config{
            .color = item.GetColor(0), .worldSize = item.frame.size });

        // All immediate paths use blend so they batch render together
        // Optimize: if you know all shapes are opaque and will be behind all transparent textures,
        // you can optimize renders by using opaque render for shapes (Careful: this will result in
        // incorrect Z-order in some cases)
        static_cast<ColorRenderer*>(result.get())->EnableBlend(!areShapesOpaque);

        return result;
    };

    id = RendererId(ImPathItemType::Rect, ImPathRenderType::Frame);
    buildRendererFuncs[id] = [this](auto& item) {
        UP<SomeRenderer> result = NEW<ColorRenderer>(ColorRenderer::Config{
            .color = item.GetColor(0), .worldSize = item.frame.size });

        auto strokeWidth = item.strokeWidth;

        auto colorRenderer = static_cast<ColorRenderer*>(result.get());
        // TODO: rethink this pattern
        colorRenderer->EnableBlend(!areShapesOpaque);
        colorRenderer->SetBuildMeshFunc([strokeWidth](auto& model) {
            QuadFrameMeshBuilder meshBuilder(model.WorldSize(), { strokeWidth, strokeWidth });
            return meshBuilder.BuildMesh();
        });

        return result;
    };

    // MARK: Polygon

    id = RendererId(ImPathItemType::Polygon, ImPathRenderType::Frame);
    buildRendererFuncs[id] = [this](auto& item) {
        UP<SomeRenderer> result = NEW<ColorRenderer>(ColorRenderer::Config{
            .color = item.GetColor(0), .worldSize = item.frame.size });

        auto colorRenderer = static_cast<ColorRenderer*>(result.get());
        colorRenderer->EnableBlend(!areShapesOpaque);
        colorRenderer->SetWorldSize(item.poly.Size());

        colorRenderer->SetBuildMeshFunc([item](auto& model) {
            PolyFrameMeshBuilder meshBuilder{ { .startCap = item.startPathCap,
                                                .endCap = item.endPathCap,
                                                .polyClose = item.polyClose,
                                                .corner = item.pathCorner,
                                                .strokeWidth = item.strokeWidth,
                                                .poly = item.poly } };
            return meshBuilder.BuildMesh();
        });

        return result;
    };

    // MARK: Capsule

    id = RendererId(ImPathItemType::Capsule, ImPathRenderType::Fill);
    buildRendererFuncs[id] = [this](auto& item) {
        UP<SomeRenderer> result = NEW<ColorRenderer>(ColorRenderer::Config{
            .color = item.GetColor(0), .worldSize = item.frame.size });

        auto itemAxis = item.axis;

        auto colorRenderer = static_cast<ColorRenderer*>(result.get());
        colorRenderer->EnableBlend(!areShapesOpaque);
        colorRenderer->SetBuildMeshFunc([itemAxis](auto& model) {
            CapsuleMeshBuilder meshBuilder{ { .axis = itemAxis, .worldSize = model.WorldSize() } };
            return meshBuilder.BuildMesh();
        });

        return result;
    };

    // MARK: RoundCorners

    id = RendererId(ImPathItemType::RoundCorners, ImPathRenderType::Fill);
    buildRendererFuncs[id] = [this](auto& item) {
        UP<SomeRenderer> result = NEW<ColorRenderer>(ColorRenderer::Config{
            .color = item.GetColor(0), .worldSize = item.frame.size });

        auto itemCorners = item.roundCorners;

        auto colorRenderer = static_cast<ColorRenderer*>(result.get());
        colorRenderer->EnableBlend(!areShapesOpaque);
        colorRenderer->SetBuildMeshFunc([itemCorners](auto& model) {
            RoundCornersMeshBuilder meshBuilder{ { .corners = itemCorners,
                                                   .worldSize = model.WorldSize() } };
            return meshBuilder.BuildMesh();
        });

        return result;
    };

    // MARK: Ellipse

    id = RendererId(ImPathItemType::Arc, ImPathRenderType::Fill);
    buildRendererFuncs[id] = [this](auto& item) {
        UP<SomeRenderer> result = NEW<ColorRenderer>(ColorRenderer::Config{
            .color = item.GetColor(0), .worldSize = item.frame.size });

        auto polyModel = CenterPolyModel::ellipse;
        polyModel.startAngle = item.startAngle;
        polyModel.angleDelta = item.angleDelta;

        auto colorRenderer = static_cast<ColorRenderer*>(result.get());
        colorRenderer->EnableBlend(!areShapesOpaque);
        colorRenderer->SetBuildMeshFunc([polyModel](auto& model) {
            CenterPolyMeshBuilder meshBuilder(model.WorldSize(), polyModel);
            return meshBuilder.BuildMesh();
        });

        return result;
    };

    id = RendererId(ImPathItemType::Arc, ImPathRenderType::Frame);
    buildRendererFuncs[id] = [this](auto& item) {
        UP<SomeRenderer> result = NEW<ColorRenderer>(ColorRenderer::Config{
            .color = item.GetColor(0), .worldSize = item.frame.size });

        auto strokeWidth = item.strokeWidth;
        auto polyModel = CenterPolyModel::ellipse;
        polyModel.startAngle = item.startAngle;
        polyModel.angleDelta = item.angleDelta;

        auto colorRenderer = static_cast<ColorRenderer*>(result.get());
        colorRenderer->EnableBlend(!areShapesOpaque);
        colorRenderer->SetBuildMeshFunc([strokeWidth, polyModel](auto& model) {
            CenterPolyFrameMeshBuilder meshBuilder(
                { .worldSize = model.WorldSize(), .strokeWidth = strokeWidth, .model = polyModel }
            );
            return meshBuilder.BuildMesh();
        });

        return result;
    };

    // MARK: Text

    id = String(ImPathItemType::Text) + String(".") + String(ImPathRenderType::Fill);
    buildRendererFuncs[id] = [this](auto& item) -> UP<SomeRenderer> {
        UP<SomeRenderer> result = NEW<TextRenderer>(TextRenderer::Config{
            .font = item.font, .text = item.text, .worldSize = item.frame.size });
        static_cast<TextRenderer*>(result.get())->SetColor(item.GetColor(0, Color::white));
        return result;
    };

    // MARK: Image

    id = String(ImPathItemType::Image) + String(".") + String(ImPathRenderType::Fill);
    buildRendererFuncs[id] = [this](auto& item) -> UP<SomeRenderer> {
        SP<SomeTexture> texture = item.texture;
        if (nullptr == texture) {
            GUARDR(owner, {})

            auto world = owner->World();
            GUARDR(world, {})

            texture = world->resources.FindTexture(item.id);
            GUARDR(texture, {})
        }

        UP<SomeRenderer> result = NEW<SpriteRenderer>(SpriteRenderer::Config{ .texture = texture });
        auto spriteRenderer = static_cast<SpriteRenderer*>(result.get());

        spriteRenderer->SetColor(item.GetColor(0, Color::white));
        spriteRenderer->model.Material()->EnableFeature(RenderFeature::Blend, !areImagesOpaque);

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

                Matrix4x4 matrix;
                if (translateItemFunc) {
                    matrix = translateItemFunc(*this, item);
                }

                for (auto& renderModel : renderModels) {
                    renderModel.ModifiableMesh() *= matrix;
                }
                AddRange(result, renderModels);

                // Keep the renderer and material in memory for the render
                renderers.push_back(std::move(renderer));
            } catch (...) {}
        }

        paths.clear();
        if (!isFrozen) {
            model.SetRenderModelsNeedBuild();
        }

        return result;
    });
}
