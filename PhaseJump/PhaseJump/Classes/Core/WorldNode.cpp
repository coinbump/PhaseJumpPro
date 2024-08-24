#include "WorldNode.h"
#include "Matrix4x4.h"
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

bool WorldNode::IsActive() const {
    return isActive && !isDestroyed;
}

void WorldNode::SetActive(bool isActive) {
    this->isActive = isActive;
}

void WorldNode::ToggleActive() {
    SetActive(!isActive);
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

Vector3 WorldNode::Scale() const {
    return transform.Scale();
}

void WorldNode::SetScale(Vector3 value) {
    transform.SetScale(value);
}

void WorldNode::SetScale(float value) {
    SetScale(Vector3::Uniform(value));
}

void WorldNode::SetScale2D(Vector2 value) {
    SetScale(Vector3(value.x, value.y, 1.0f));
}

void WorldNode::SetScale2D(float value) {
    SetScale(Vector3(value, value, 1.0f));
}

Vector3 WorldNode::LocalScale() const {
    return Scale();
}

void WorldNode::SetLocalScale(Vector3 value) {
    SetScale(value);
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
