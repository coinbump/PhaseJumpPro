#include "WorldNode.h"
#include "Colliders2D.h"
#include "Color.h"
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
#include <TSMatrix4D.h>

using namespace std;
using namespace PJ;

WorldNode::WorldNode(String name) :
    transform(*this),
    name(name) {}

void WorldNode::Destroy(float countdown) {
    if (countdown == 0) {
        destroyCountdown = 0;
        isDestroyed = true;
        return;
    }

    destroyCountdown = countdown;
}

void WorldNode::OnDestroy() {
    for (auto& component : components) {
        component->OnDestroy();
    }
}

WorldNode* WorldNode::Parent() const {
    return parent;
}

WorldNode::NodeList const& WorldNode::ChildNodes() const {
    return children;
}

WorldNode::NodeList const& WorldNode::Children() const {
    return children;
}

bool WorldNode::IsAwake() const {
    return life.IsAwake();
}

bool WorldNode::IsStarted() const {
    return life.IsStarted();
}

bool WorldNode::IsDestroyed() const {
    return isDestroyed;
}

std::size_t WorldNode::ChildCount() {
    return children.size();
}

bool WorldNode::IsEnabled() const {
    return isEnabled && !isDestroyed;
}

WorldNode& WorldNode::Enable(bool value) {
    isEnabled = value;
    return *this;
}

void WorldNode::ToggleEnable() {
    Enable(!isEnabled);
}

// MARK: Add and remove

WorldNode::ComponentList const& WorldNode::Components() const {
    return components;
}

void WorldNode::Add(SP<SomeWorldComponent> component) {
    GUARD(component)

    if (nullptr != component->owner) {
        PJLog("ERROR. Can't add parented component");
        return;
    }

    PJ::Add(components, component);
    component->owner = this;

    // If this node is started, forward life cycle events to the component
    if (life.IsAwake()) {
        component->CheckedAwake();
    }
    if (life.IsStarted()) {
        component->CheckedStart();
    }
}

void WorldNode::Add(SP<WorldNode> node) {
    GUARD(node)

    if (nullptr != node->Parent()) {
        PJLog("ERROR. Can't add a previously parented node");
        return;
    }

    PJ::Add(children, node);
    node->parent = this;

    NodeList subgraph;
    CollectBreadthFirstTree(node, subgraph);

    // Set the world. Lifecycle events occur in world update
    for (auto& subNode : subgraph) {
        subNode->world = world;
    }
}

void WorldNode::Remove(SP<WorldNode> node) {
    GUARD(node)

    if (this != node->parent) {
        PJLog("ERROR. Can't remove un-parented node");
        return;
    }

    node->parent = nullptr;

    NodeList subgraph;
    CollectBreadthFirstTree(node, subgraph);
    for (auto& subNode : subgraph) {
        subNode->world = nullptr;
    }

    PJ::Remove(children, node);
}

void WorldNode::RemoveAllChildren() {
    for (auto& child : children) {
        child->parent = nullptr;

        NodeList subgraph;
        CollectBreadthFirstTree(child, subgraph);
        for (auto& subNode : subgraph) {
            subNode->world = nullptr;
        }
    }
    children.clear();
}

void WorldNode::Remove(SP<SomeWorldComponent> component) {
    GUARD(component)

    component->owner = nullptr;

    PJ::Remove(components, component);
}

void WorldNode::RemoveAllComponents() {
    auto iterComponents = components;
    std::for_each(
        iterComponents.begin(), iterComponents.end(),
        [&](SP<SomeWorldComponent>& component) { component->owner = nullptr; }
    );
    components.clear();
}

Matrix4x4 WorldNode::ModelMatrix() const {
    Matrix4x4 result;
    result.LoadIdentity();

    GUARDR(world, result)
    return world->WorldModelMatrix(*this);
}

Vector3 WorldNode::Scale() const {
    return transform.Scale();
}

WorldNode& WorldNode::SetScale(Vector3 value) {
    transform.SetScale(value);
    return *this;
}

WorldNode& WorldNode::SetScale(float value) {
    return SetScale(Vector3::Uniform(value));
}

WorldNode& WorldNode::SetScale2D(Vector2 value) {
    SetScale(Vector3(value.x, value.y, 1.0f));
    return *this;
}

WorldNode& WorldNode::SetScale2D(float value) {
    return SetScale(Vector3(value, value, 1.0f));
}

Vector3 WorldNode::LocalScale() const {
    return Scale();
}

WorldNode& WorldNode::SetLocalScale(Vector3 value) {
    return SetScale(value);
}

World* WorldNode::World() const {
    return world;
}

void WorldNode::CheckedAwake() {
    life.CheckedAwake([this]() {
        auto iterComponents = components;
        for (auto& component : iterComponents) {
            component->CheckedAwake();
        }
    });
}

void WorldNode::CheckedStart() {
    life.CheckedStart([this]() {
        auto iterComponents = components;
        for (auto& component : iterComponents) {
            component->CheckedStart();
        }
    });
}

void WorldNode::OnUpdate(TimeSlice time) {
    if (destroyCountdown > 0) {
        destroyCountdown -= time.delta;
        if (destroyCountdown <= 0) {
            isDestroyed = true;
        }
    }

    for (auto const& component : components) {
        if (!component->IsEnabled()) {
            continue;
        }
        component->OnUpdate(time);
    }
}

// TODO: new QuickBuild namespace?
// MARK: - Quick build

WorldNode& WorldNode::AddCircle(float radius, Color color) {
    this->AddComponent<ColorRenderer>(color, Vector2(radius * 2, radius * 2))
        .SetMeshBuilderFunc([](RendererModel const& model) {
            return EllipseMeshBuilder(model.WorldSize()).BuildMesh();
        });

    return *this;
}

WorldNode& WorldNode::AddDrag(OnDragUpdateFunc onDragUpdateFunc) {
    this->AddComponent<DragHandler2D>().SetOnDragUpdateFunc(onDragUpdateFunc);

    if (!this->TypeComponent<SomeCollider2D>()) {
        PJLog("WARNING. Drag requires a collider");
    }

    return *this;
}

WorldNode& WorldNode::AddSquareCollider(float size) {
    auto& polyC = this->AddComponent<PolygonCollider2D>();
    polyC.poly = Polygon::MakeSquare(size);

    return *this;
}

WorldNode& WorldNode::AddRectCollider(Vector2 size) {
    auto& polyC = this->AddComponent<PolygonCollider2D>();
    polyC.poly = Polygon::MakeRect(size);

    return *this;
}

WorldNode& WorldNode::AddCircleCollider(float radius) {
    AddComponent<CircleCollider2D>(radius);
    return *this;
}

void _AddSlider(
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

WorldNode&
WorldNode::AddSlider(String name, Vector2 worldSize, std::function<void(float)> valueFunc) {
    GUARDR(world, *this)

    auto designSystem = world->designSystem;
    GUARDR_LOG(designSystem, *this, "Missing design system")

    auto trackTexture = designSystem->Texture(UIElement::SliderTrack);
    GUARDR_LOG(trackTexture, *this, "Missing slider track texture")

    auto thumbTexture = designSystem->Texture(UIElement::SliderThumb);
    GUARDR_LOG(thumbTexture, *this, "Missing slider thumb texture")

    _AddSlider(
        *world, *this, *designSystem, trackTexture, thumbTexture, name, worldSize, Axis2D::X,
        valueFunc
    );
    return *this;
}

WorldNode&
WorldNode::AddSliderVertical(String name, Vector2 worldSize, std::function<void(float)> valueFunc) {
    GUARDR(world, *this)

    auto designSystem = world->designSystem;
    GUARDR_LOG(designSystem, *this, "Missing design system")

    // FUTURE: support UV rotation so we don't have to duplicate textures
    auto trackTexture = designSystem->Texture(UIElement::SliderVerticalTrack);
    GUARDR_LOG(trackTexture, *this, "Missing slider track texture")

    auto thumbTexture = designSystem->Texture(UIElement::SliderVerticalThumb);
    GUARDR_LOG(thumbTexture, *this, "Missing slider thumb texture")

    _AddSlider(
        *world, *this, *designSystem, trackTexture, thumbTexture, name, worldSize, Axis2D::Y,
        valueFunc
    );
    return *this;
}

/*
 FUTURE: Quick Build options:
 */
