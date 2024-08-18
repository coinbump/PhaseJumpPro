#ifndef PJWORLDNODE_H
#define PJWORLDNODE_H

#include "AcyclicGraphNode.h"
#include "GeoTransform.h"
#include "GraphNodeTool.h"
#include "List.h"
#include "Log.h"
#include "SomeUIEvent.h"
#include "SomeWorldComponent.h"
#include "Utils.h"
#include "VectorList.h"
#include "WorldNodeTransform.h"
#include "WorldPartLife.h"
#include <memory>

// TODO: Unit tests
// CODE REVIEW: /23
namespace PJ {
    class LocalPosition;
    class World;

    /// A node in the world graph
    class WorldNode : public AcyclicGraphNode<> {
    public:
        using ComponentSharedPtr = SP<SomeWorldComponent>;
        using ComponentPtr = ComponentSharedPtr const&;

    protected:
        VectorList<ComponentSharedPtr> components;
        float destroyCountdown = 0;
        bool isDestroyed = false;
        bool isActive = true;

        WorldPartLife life;

    public:
        using NodeTransform = WorldNodeTransform;

        // Don't use this directly, use World(), because this is only set in the
        // root node
        World* world = nullptr;

        String name;

        using Base = AcyclicGraphNode<>;

        // TODO: does this have to be a pointer?
        UP<NodeTransform> transform = std::make_unique<NodeTransform>(*this);

        WorldNode(String name = "") :
            name(name) {}

        bool IsDestroyed() const {
            return isDestroyed;
        }

        std::size_t ChildCount() {
            return edges.size();
        }

        VectorList<ComponentSharedPtr> const& Components() const {
            return components;
        }

        World* World() const;

        void Add(ComponentPtr component) {
            if (nullptr != component->owner) {
                PJLog("ERROR. Can't add parented component");
                return;
            }

            GUARD(component)

            components.Add(component);
            component->owner = this;

            // If this node is started, forward life cycle events to the component
            if (life.IsAwake()) {
                component->CheckedAwake();
            }
            if (life.IsStarted()) {
                component->CheckedStart();
            }
        }

        template <class T>
        SP<T> AddComponent() {
            auto component = MAKE<T>();
            Add(component);
            return component;
        }

        // TODO: return something here? Edge? iterator?
        void Add(SP<WorldNode> node) {
            GUARD(node)

            if (nullptr != node->Parent()) {
                PJLog("ERROR. Can't add a previously parented node");
                return;
            }

            AddEdge(node);

            // Assign world to children and descendants
            GraphNodeTool<> nodeTool;
            auto nodeList = nodeTool.CollectDepthFirstGraph(node);
            for (auto& node : nodeList) {
                SCAST<WorldNode>(node)->world = world;
            }
        }

        void Remove(SP<WorldNode> node) {
            GUARD(node)

            if (this != node->Parent().get()) {
                PJLog("ERROR. Can't remove un-parented node");
                return;
            }

            node->world = nullptr;
            RemoveEdgesTo(*node);
        }

        void Remove(SP<SomeWorldComponent> component) {
            GUARD(component)

            component->owner = nullptr;

            PJ::Remove(components, component);
        }

        void RemoveAllComponents() {
            auto iterComponents = components;
            std::for_each(
                iterComponents.begin(), iterComponents.end(),
                [&](SP<SomeWorldComponent>& component) { Remove(component); }
            );
        }

        template <class T>
        SP<T> TypeComponent() const {
            for (auto& component : components) {
                auto typeComponent = DCAST<T>(component);
                if (typeComponent) {
                    return typeComponent;
                }
            }

            return nullptr;
        }

        template <class T>
        VectorList<SP<T>> TypeComponents() const {
            VectorList<SP<T>> result;

            for (auto& component : components) {
                auto typeComponent = DCAST<T>(component);
                if (typeComponent) {
                    result.Add(typeComponent);
                }
            }

            return result;
        }

        // This can't be const because Graph collection requires non const
        // results
        template <class T>
        VectorList<SP<T>> TypeComponentsInChildren() {
            VectorList<SP<T>> result;

            GraphNodeTool graphNodeTool;
            auto graph =
                graphNodeTool.CollectBreadthFirstGraph(SCAST<SomeGraphNode>(this->shared_from_this()
                ));

            for (auto& node : graph) {
                auto worldNode = SCAST<WorldNode>(node);
                auto components = worldNode->Components();

                for (auto& component : components) {
                    auto typeComponent = DCAST<T>(component);
                    if (typeComponent) {
                        result.Add(typeComponent);
                    }
                }
            }

            return result;
        }

        void Destroy(float countdown = 0) {
            if (countdown == 0) {
                destroyCountdown = 0;
                isDestroyed = true;
                return;
            }

            destroyCountdown = countdown;
        }

        bool IsAwake() const {
            return life.IsAwake();
        }

        bool IsStarted() const {
            return life.IsStarted();
        }

        /// Called before Start
        void CheckedAwake();

        /// Called after Awake
        void CheckedStart();

        bool IsActive() const {
            return isActive && !isDestroyed;
        }

        void SetActive(bool isActive) {
            this->isActive = isActive;
        }

        virtual void OnDestroy() {
            for (auto& component : components) {
                component->OnDestroy();
            }
        }

        void OnUpdate(TimeSlice time) override;

        VectorList<SP<WorldNode>> ChildNodes() const {
            VectorList<SP<WorldNode>> result;

            for (auto& edge : edges) {
                result.Add(SCAST<WorldNode>(edge->toNode->Value()));
            }

            return result;
        }

        // Convenience names
        template <class T>
        SP<T> GetComponent() const {
            return TypeComponent<T>();
        }

        // TODO: what about disabled components??
        template <class T>
        VectorList<SP<T>> GetComponents() const {
            return TypeComponents<T>();
        }

        template <class T>
        VectorList<SP<T>> GetComponentsInChildren() {
            return TypeComponentsInChildren<T>();
        }

        // MARK: Convenience Scale

        Vector3 Scale() const {
            GUARDR(transform, Vector3::one);
            return transform->Scale();
        }

        void SetScale(Vector3 value) {
            GUARD(transform);
            transform->SetScale(value);
        }

        void SetScale(float value) {
            SetScale(Vector3::Uniform(value));
        }

        void SetScale2D(Vector2 value) {
            SetScale(Vector3(value.x, value.y, 1.0f));
        }

        void SetScale2D(float value) {
            SetScale(Vector3(value, value, 1.0f));
        }

        Vector3 LocalScale() const {
            return Scale();
        }

        void SetLocalScale(Vector3 value) {
            SetScale(value);
        }
    };
} // namespace PJ

#endif
