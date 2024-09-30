#include "WorldNode.h"
#include "Color.h"
#include "SomeRenderer.h"
#include "World.h"
#include <TSMatrix4D.h>

using namespace std;
using namespace PJ;

using This = WorldNode;

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

void WorldNode::Restore() {
    destroyCountdown = 0;
    isDestroyed = false;
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
        PJ::Log("ERROR. Can't add parented component");
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
    GUARD_LOG(nullptr == node->Parent(), "ERROR. Can't add a previously parented node")

    PJ::Add(children, node);
    node->parent = this;

    NodeList subgraph;
    CollectBreadthFirstTree(node, subgraph);

    // Set the world. Lifecycle events (Start, Awake) occur in world update
    for (auto& subNode : subgraph) {
        subNode->world = world;
    }

    AddChildNodeEvent addChildNodeEvent(node);

    // Let components know there is a new child node
    // Some components need this information (Example: layouts)
    for (auto& component : components) {
        component->Signal(SignalId::AddChildNode, addChildNodeEvent);
    }
}

void WorldNode::Remove(SP<WorldNode> node) {
    GUARD(node)
    GUARD_LOG(node->Parent() == this, "ERROR. Can't remove un-parented node")

    node->parent = nullptr;

    NodeList subgraph;
    CollectBreadthFirstTree(node, subgraph);
    for (auto& subNode : subgraph) {
        subNode->world = nullptr;
    }

    PJ::Remove(children, node);
}

void WorldNode::Remove(WorldNode& node) {
    GUARD_LOG(node.Parent() == this, "ERROR. Can't remove un-parented node")
    Remove(SCAST<WorldNode>(node.shared_from_this()));
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

    updatables.OnUpdate(time);
}

Vector3 WorldNode::LocalToWorld(Vector3 localPos) {
    GUARDR(world, localPos)

    auto localToWorldMatrix = world->WorldModelMatrix(*this);
    auto result = localToWorldMatrix.MultiplyPoint(localPos);

    return result;
}

Vector3 WorldNode::WorldToLocal(Vector3 worldPos) {
    GUARDR(world, worldPos)

    auto localToWorldMatrix = world->WorldModelMatrix(*this);
    localToWorldMatrix.Inverse();

    auto result = localToWorldMatrix.MultiplyPoint(worldPos);
    return result;
}

float WorldNode::Opacity() const {
    auto renderer = TypeComponent<SomeRenderer>();
    GUARDR(renderer, 1.0f)

    return renderer->GetColor().a;
}

This& WorldNode::SetOpacity(float value) {
    auto renderer = TypeComponent<SomeRenderer>();
    GUARDR(renderer, *this)

    renderer->SetAlpha(value);
    return *this;
}
