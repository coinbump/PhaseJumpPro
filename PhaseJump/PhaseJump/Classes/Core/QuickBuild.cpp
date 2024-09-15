#include "QuickBuild.h"
#include "Colliders2D.h"
#include "ColorRenderer.h"
#include "DragHandler2D.h"
#include "EllipseMeshBuilder.h"
#include "Matrix4x4.h"
#include "OrthoCamera.h"
#include "SlicedTextureRenderer.h"
#include "SliderControl.h"
#include "SomeCollider2D.h"
#include "SpriteRenderer.h"
#include "Theme.h"
#include "World.h"

using namespace std;
using namespace PJ;

QuickBuild& QuickBuild::Circle(float radius, Color color) {
    auto component = &Node()
                          .AddComponent<ColorRenderer>(color, Vector2(radius * 2, radius * 2))
                          .SetBuildMeshFunc([](RendererModel const& model) {
                              return EllipseMeshBuilder(model.WorldSize()).BuildMesh();
                          });

    components.push_back(component);
    return *this;
}

QuickBuild& QuickBuild::Drag(OnDragUpdateFunc onDragUpdateFunc) {
    auto component = &Node().AddComponent<DragHandler2D>().SetOnDragUpdateFunc(onDragUpdateFunc);

    components.push_back(component);

    // Add basic drag collider if it's missing
    if (!Node().TypeComponent<SomeCollider2D>()) {
        auto worldSizeable = Node().TypeComponent<WorldSizeable>();
        if (worldSizeable) {
            auto worldSize = worldSizeable->WorldSize();
            if (worldSize) {
                return RectCollider((Vector2)*worldSize);
            }
        }
    }

    return *this;
}

QuickBuild& QuickBuild::SquareCollider(float size) {
    auto& polyC = Node().AddComponent<PolygonCollider2D>();
    polyC.poly = Polygon::MakeSquare(size);

    components.push_back(&polyC);
    return *this;
}

QuickBuild& QuickBuild::RectCollider(Vector2 size) {
    auto& polyC = Node().AddComponent<PolygonCollider2D>();
    polyC.poly = Polygon::MakeRect(size);

    components.push_back(&polyC);
    return *this;
}

QuickBuild& QuickBuild::CircleCollider(float radius) {
    auto component = &Node().AddComponent<CircleCollider2D>(radius);

    components.push_back(component);
    return *this;
}

void QuickBuild::AddSlider(
    World& world, WorldNode& parent, DesignSystem& designSystem, SP<SomeTexture> trackTexture,
    SP<SomeTexture> thumbTexture, String name, Vector2 worldSize, Axis2D axis,
    std::function<void(float)> valueFunc
) {
    GUARD(trackTexture && thumbTexture)

    SlicedTextureRenderer::SlicePoints slicePoints =
        designSystem.TagValue<SlicedTextureRenderer::SlicePoints>(
            UIElement::SliderTrack, UITag::SlicePoints
        );

    auto& sliderNode = parent.AddNode("Slider track");

    sliderNode.AddComponent<SlicedTextureRenderer>(trackTexture, vec2Zero, slicePoints);
    auto& sliderControl = sliderNode.AddComponent<SliderControl>(axis);
    components.push_back(&sliderControl);

    float endCapSize = designSystem.TagValue<float>(UIElement::SliderTrack, UITag::EndCapSize);
    sliderControl.SetEndCapSize(endCapSize).SetFrame(Rect({ 0, 0 }, worldSize));
    auto& thumbNode = sliderNode.AddNode("Slider thumb");

    thumbNode.AddComponent<PolygonCollider2D>().SetPoly(
        Polygon::MakeRect(Vector2(thumbTexture->size.x, thumbTexture->size.y))
    );

    thumbNode.AddComponent<SpriteRenderer>(thumbTexture);

    if (valueFunc) {
        // Subscribe to value and store subscription
        // Careful: watch out for strong reference cycles when using smart pointers
        sliderNode.cancellables.insert(sliderControl.Value().Receive(valueFunc));
    }
}

QuickBuild&
QuickBuild::Slider(String name, Vector2 worldSize, std::function<void(float)> valueFunc) {
    if (worldSize.y > worldSize.x) {
        return SliderVertical(name, worldSize, valueFunc);
    }

    auto world = Node().World();
    GUARDR(world, *this)

    auto designSystem = world->designSystem;
    GUARDR_LOG(designSystem, *this, "Missing design system")

    auto trackTexture = designSystem->Texture(UIElement::SliderTrack);
    GUARDR_LOG(trackTexture, *this, "Missing slider track texture")

    auto thumbTexture = designSystem->Texture(UIElement::SliderThumb);
    GUARDR_LOG(thumbTexture, *this, "Missing slider thumb texture")

    AddSlider(
        *world, Node(), *designSystem, trackTexture, thumbTexture, name, worldSize, Axis2D::X,
        valueFunc
    );
    return *this;
}

QuickBuild&
QuickBuild::SliderVertical(String name, Vector2 worldSize, std::function<void(float)> valueFunc) {
    auto world = Node().World();
    GUARDR(world, *this)

    auto designSystem = world->designSystem;
    GUARDR_LOG(designSystem, *this, "Missing design system")

    // FUTURE: support UV rotation so we don't have to duplicate textures
    auto trackTexture = designSystem->Texture(UIElement::SliderVerticalTrack);
    GUARDR_LOG(trackTexture, *this, "Missing slider track texture")

    auto thumbTexture = designSystem->Texture(UIElement::SliderVerticalThumb);
    GUARDR_LOG(thumbTexture, *this, "Missing slider thumb texture")

    AddSlider(
        *world, Node(), *designSystem, trackTexture, thumbTexture, name, worldSize, Axis2D::Y,
        valueFunc
    );
    return *this;
}

QuickBuild& QuickBuild::AndPrefab(String id) {
    auto world = Node().World();
    GUARDR(world, *this)

    auto prefabNode = world->PrefabMake(id);
    GUARDR(prefabNode, *this)

    Node().Add(prefabNode);
    Push(*prefabNode);

    return *this;
}

/*
 FUTURE: Quick Build options:
 */
