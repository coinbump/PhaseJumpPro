#pragma once

#include "EventWorldSystem.h"
#include "OrthoCamera.h"
#include "Prefab.h"
#include "RateLimiter.h"
#include "ResourceCatalog.h"
#include "SomeRenderContext.h"
#include "SomeUIEventPoller.h"
#include "Updatable.h"
#include "Utils.h"
#include "WorldComponent.h"
#include <memory>

// CODE REVIEW: ?/23
namespace PJ {
    class WorldNode;
    class SomeWorldSystem;
    class SomeCamera;
    class RenderMaterial;
    class Font;
    class DesignSystem;
    class FontSpec;
    class SomeShaderProgram;

    /**
     Container for the ECS system that drives Phase Jump Pro

     Each window has a world. Each world contains a node tree with nodes. Each node contains
     components that implement specific behavior. A renderer component produces render models, which
     are sent to the render system. Those render models are then processed by the render
     engine into GPU commands

     If you don't want to use ECS, subclass world or create a single node + renderer.
     */
    class World : public Base {
    public:
        DELETE_COPY(World)

    protected:
        using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;

        /// Stores time point to calculate FPS
        std::optional<TimePoint> fpsCheckTimePoint;

        /// For FPS calculation
        uint64_t renderFrameCount{};

        /// Internal, list for update
        WorldNode::NodeList updateList;

        /// Systems attached to this world
        VectorList<SP<SomeWorldSystem>> systems;

        /// Main camera for this world
        WP<SomeCamera> mainCamera;

        /// If true, the world will not receive update events
        bool isPaused = false;

        /// Rate limits renders
        RateLimiter renderLimiter{ 1.0f / 240.0f };

        /// ECS root node. Child nodes add components that implement specific behaviors for each
        /// node
        SP<WorldNode> root = MAKE<WorldNode>("Root");

    public:
        /// Number of pixels per point
        /// Example: on a Retina screen, each point represents 2 pixels, this value would be 2
        float uiScale = 1;

        /// Stores objects that need time updates
        Updatables updatables;

        /// Use to display render stats like fps, render count, etc.
        Tags renderStats;

        /// Stores resources loaded for this world
        ResourceCatalog resources;

        /// Main render context attached to the window for this world
        SP<SomeRenderContext> renderContext;

        /// Polls for UI events
        UP<SomeUIEventPoller> uiEventPoller;

        /// Design system for quick build UI
        /// Allows us to add theme components with ViewBuilder
        SP<DesignSystem> designSystem;

        /// Render materials that can be shared between objects
        UnorderedMap<String, SP<RenderMaterial>> renderMaterials;

        /// Prefabs, mapped by id
        UnorderedMap<String, SP<Prefab>> prefabs;

        /// If true, render FPS is capped
        bool isRenderRateLimited = true;

        /// Update time deltas are modified by this value
        float timeScale = 1;

        World();

        WorldNode* Root() const {
            return root.get();
        }

        void SetRenderRate(float value) {
            renderLimiter.core.rate = 1.0f / value;
        }

        void SetIsRenderRateLimited(bool value) {
            isRenderRateLimited = value;
        }

        /// @return Returns the first system that matches the type
        template <class Type>
        SP<Type> TypeSystem() {
            auto found =
                std::find_if(systems.begin(), systems.end(), [](SP<SomeWorldSystem> system) {
                    return DCAST<Type>(system) != nullptr;
                });

            GUARDR(found != systems.end(), nullptr)
            return DCAST<Type>(*found);
        }

        template <class Type>
        SP<Type> GetSystem() {
            return TypeSystem<Type>();
        }

        virtual SomeCamera* MainCamera();

        /// @return Returns a matrix that transforms this node's vertices in local space (position +
        /// offset + scale + rotation)
        virtual Matrix4x4 LocalModelMatrix(WorldNode const& node);

        /// @return Returns a matrix that transforms this node's vertices into word space
        virtual Matrix4x4 WorldModelMatrix(WorldNode const& node);

        /// Sends update event to world
        void OnUpdate(TimeSlice time);

        /// Renders if the rate limiter allows it
        virtual void Render();

        /// Renders immediately
        virtual void RenderNow();

        /// Adds a node to the root node
        void Add(SP<WorldNode> node);

        /// Adds a system
        void Add(SP<SomeWorldSystem> system);

        /// Adds a node with constructor arguments
        template <typename... Arguments>
        WorldNode& AddNode(Arguments... args) {
            SP<WorldNode> result = MAKE<WorldNode>(args...);
            Add(result);
            return *result;
        }

        /// Shorthand for adding a node
        template <typename... Arguments>
        constexpr WorldNode& And(Arguments... args) {
            return AddNode(args...);
        }

        /// Adds a node at a specific position
        template <typename... Arguments>
        WorldNode& AddNodeAt(Vector3 pos, Arguments... args) {
            SP<WorldNode> result = MAKE<WorldNode>(args...);
            Add(result);
            result->SetLocalPosition(pos);
            return *result;
        }

        /// @return Returns the list of systems attached to this world
        VectorList<SP<SomeWorldSystem>>& Systems() {
            return systems;
        }

        /// @return Returns the list of systems attached to this world
        VectorList<SP<SomeWorldSystem>> const& Systems() const {
            return systems;
        }

        /// @return Returns true if the world is paused
        bool IsPaused() const;

        /// Pauses the world so it will not receive any update events
        void Pause();

        /// Unpauses the world so it can receive update events
        void Play();

        /// Loads a prefab into the specified node, if the prefab exists
        void LoadPrefab(String id, WorldNode& node);

        /// Removes a system
        void Remove(SomeWorldSystem& system);

        /// Removes a list of systems
        void Remove(VectorList<SP<SomeWorldSystem>> systems);

        /// Removes all systems
        void RemoveAllSystems();

        /// Remove all nodes except for the root node
        void RemoveAllNodes();

        void SetRenderContext(SP<SomeRenderContext> renderContext);

        virtual void ProcessUIEvents(UIEventList const& uiEvents);

    protected:
        // MARK: Base

        void OnGo() override;
    };

    /// @return Returns the local position in this component's owner space for the screen position
    LocalPosition ScreenToLocal(SomeWorldComponent& component, ScreenPosition screenPos);

    /// @return Returns the world positionin this component's owner space for the screen position
    WorldPosition ScreenToWorld(SomeWorldComponent& component, ScreenPosition screenPos);

} // namespace PJ
