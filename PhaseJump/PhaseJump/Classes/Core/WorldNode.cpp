#include "WorldNode.h"
#include "Color.h"
#include "SomeRenderer.h"
#include "World.h"
#include <TSMatrix4D.h>

using namespace std;
using namespace PJ;

using This = WorldNode;

WorldNode::WorldNode(Config config) :
    transform(*this),
    tree(*this) {
    core.id = config.id;
    core.name = config.name;
}

WorldNode::WorldNode(String name) :
    WorldNode(Config{ .name = name }) {}

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

WorldNode::NodeList const& WorldNode::ChildNodes() const {
    return Children();
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
    return tree.ChildCount();
}

bool WorldNode::IsEnabled() const {
    return !isDisabled && !isDestroyed;
}

WorldNode& WorldNode::Enable(bool value) {
    isDisabled = !value;
    return *this;
}

void WorldNode::ToggleEnable() {
    Enable(isDisabled);
}

bool WorldNode::IsVisible() const {
    return !isInvisible && !isDestroyed && IsStarted();
}

WorldNode& WorldNode::SetIsVisible(bool value) {
    isInvisible = !value;
    return *this;
}

void WorldNode::ToggleIsVisible() {
    SetIsVisible(isInvisible);
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

    if (!IsEmpty(component->_core.SignalHandlers())) {
        isListener = true;
    }
}

// Keep for testing single component ECS system for compatibility with Godot
// #define PJ_ECS_SINGLE_COMPONENT

void WorldNode::AddAssociate(SP<SomeWorldComponent> component) {
#ifdef PJ_ECS_SINGLE_COMPONENT
    auto child = MAKE<WorldNode>();
    Add(child);
    child->Add(component);
#else
    Add(component);
#endif
}

void WorldNode::Add(SP<WorldNode> node) {
    Insert(node, tree.ChildCount());
}

void WorldNode::Insert(SP<WorldNode> node, size_t index) {
    GUARD(node)
    GUARD_LOG(nullptr == node->Parent(), "ERROR. Can't add a previously parented node")

    tree.Insert(node, index);

    VectorList<WorldNode*> subgraph;
    CollectBreadthFirstTree(node.get(), subgraph);

    // Set the world. Lifecycle events (Start, Awake) occur in world update
    for (auto& subNode : subgraph) {
        subNode->world = world;
    }

    // The id map allows us to quickly access identifiable children without constantly
    // filtering the entire child node list
    auto id = node->Id();
    if (!IsEmpty(id)) {
        childMap.insert_or_assign(id, node.get());
    }

    AddChildNodeEvent childNodeEvent(node);

    // Let components know there is a new child node
    // Some components need this information (layouts)
    for (auto& component : components) {
        component->Signal(SignalId::AddChildNode, childNodeEvent);
    }
}

SP<WorldNode> WorldNode::Move(WorldNode& node) {
    SP<WorldNode> result = SCAST<WorldNode>(node.shared_from_this());

    tree.Remove(node);

    auto id = node.Id();
    if (!IsEmpty(id)) {
        childMap.erase(id);
    }

    RemoveChildNodeEvent childNodeEvent(result);

    // Let components know we removed a child node
    // Some components need this information (layouts)
    for (auto& component : components) {
        component->Signal(SignalId::RemoveChildNode, childNodeEvent);
    }

    return result;
}

void WorldNode::Remove(SP<WorldNode> node) {
    GUARD(node)
    GUARD_LOG(node->Parent() == this, "ERROR. Can't remove un-parented node")

    auto world = World();
    if (world && world->IsRemoveNodesLocked()) {
        // If this happens, use Destroy instead
        PJ::Log("ERROR. Can't remove nodes");
        return;
    }

    VectorList<WorldNode*> subgraph;
    CollectBreadthFirstTree(node.get(), subgraph);
    for (auto& subNode : subgraph) {
        subNode->world = nullptr;
    }

    tree.Remove(*node);

    auto id = node->Id();
    if (!IsEmpty(id)) {
        childMap.erase(id);
    }

    RemoveChildNodeEvent childNodeEvent(node);

    // Let components know we removed a child node
    // Some components need this information (layouts)
    for (auto& component : components) {
        component->Signal(SignalId::RemoveChildNode, childNodeEvent);
    }
}

void WorldNode::Remove(WorldNode& node) {
    Remove(SCAST<WorldNode>(node.shared_from_this()));
}

void WorldNode::RemoveAllChildren() {
    auto world = World();
    if (world && world->IsRemoveNodesLocked()) {
        // If this happens, use Destroy instead
        PJ::Log("ERROR. Can't remove nodes");
        return;
    }

    for (auto& child : tree.Children()) {
        VectorList<WorldNode*> subgraph;
        CollectBreadthFirstTree(child.get(), subgraph);
        for (auto& subNode : subgraph) {
            subNode->world = nullptr;
        }
    }
    tree.RemoveAllChildren(true);
    childMap.clear();
}

void WorldNode::DestroyAllChildren() {
    for (auto& child : tree.Children()) {
        child->Destroy();
    }
}

void WorldNode::Remove(SomeWorldComponent& component) {
    component.owner = nullptr;

    RemoveFirstIf(components, [&](auto& componentPtr) { return componentPtr.get() == &component; });
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

World* WorldNode::World() const {
    return world;
}

void WorldNode::CheckedAwake() {
    life.CheckedAwake([this]() {
        auto iterComponents = components;
        for (auto& component : iterComponents) {
            // Skip components that were removed by another component in this loop
            GUARD_CONTINUE(component->owner)

            component->CheckedAwake();
        }
    });
}

void WorldNode::CheckedStart() {
    life.CheckedStart([this]() {
        auto iterComponents = components;
        for (auto& component : iterComponents) {
            // Skip components that were removed by another component in this loop
            GUARD_CONTINUE(component->owner)

            component->CheckedStart();
        }
    });
}

auto iterComponents = [](VectorList<SP<SomeWorldComponent>> const& components,
                         std::function<void(SomeWorldComponent*)> func) {
    // Avoid iterate mutation crash
    auto iterComponents = components;
    for (auto const& component : iterComponents) {
        // Skip components that were removed by another component in this loop
        GUARD_CONTINUE(component->owner)

        // Skip disabled components
        GUARD_CONTINUE(component->IsEnabled())

        func(component.get());
    }
};

void WorldNode::OnUpdate(TimeSlice time) {
    if (destroyCountdown > 0) {
        destroyCountdown -= time.delta;
        if (destroyCountdown <= 0) {
            isDestroyed = true;
            return;
        }
    }

    iterComponents(components, [&](auto component) { component->OnUpdate(time); });

    updatables.OnUpdate(time);
}

void WorldNode::LateUpdate() {
    iterComponents(components, [&](auto component) { component->LateUpdate(); });
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
    auto renderer = TypeComponent<SomeMaterialRenderer>();
    GUARDR(renderer, 1.0f)

    return renderer->GetColor().a;
}

This& WorldNode::SetOpacity(float value) {
    auto renderer = TypeComponent<SomeMaterialRenderer>();
    GUARDR(renderer, *this)

    renderer->SetAlpha(value);
    return *this;
}

bool WorldNode::Contains(SomeWorldComponent& value) {
    return std::find_if(components.begin(), components.end(), [&](auto& component) {
               return component.get() == &value;
           }) != components.end();
}

void WorldNode::Signal(String signalId, SomeSignal const& signal) {
    auto iterComponents = Components();
    std::for_each(iterComponents.begin(), iterComponents.end(), [&](auto& component) {
        GUARD(component->IsEnabled())
        component->Signal(signalId, signal);
    });
}
