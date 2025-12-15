#pragma once

#include "DesignSystem.h"
#include "EventWorldSystem.h"
#include "OrthoCamera.h"
#include "Prefab.h"
#include "RateLimiter.h"
#include "ResourceCatalog.h"
#include "SomeRenderContext.h"
#include "SomeRenderEngine.h"
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
    class SomePlatformWindow;

    /**
     Container for the system that drives Phase Jump Pro

     Each window has a world. Each world contains a node tree with nodes. Each node contains
     components that implement specific behavior. A renderer component produces render models, which
     are sent to the render system. Those render models are then processed by the render
     engine into GPU commands

     Architecture Decisions
     - Shared pointer vs unique pointer. Shared pointer for core collections was chosen for
     convenience. Unique pointer is more performant but requires more work to use properly. Shared
     pointer mimics modern reference counted languages like C# and Swift.

     Best practices:
     - Optimize: Avoid copying shared pointers (slow)
     - Optimize: Avoid dynamic_pointer_cast or DCAST, use dynamic_cast instead
     - Vector lists are usually preferred over linked lists
     - Use unique pointer over shared pointer to indicate single ownership
     */
    class World : public Base {
    public:
        DELETE_COPY(World)

    protected:
        using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;
        using SystemList = VectorList<SP<SomeWorldSystem>>;

        /// Stores time point to calculate FPS
        std::optional<TimePoint> fpsCheckTimePoint;

        /// Internal. Frame render count for FPS calculation
        uint64_t renderFrameCount{};

        /// Systems attached to this world
        SystemList systems;

        /// Cached main camera for this world
        mutable WP<SomeCamera> mainCamera;

        /// If true, the world will not receive update events
        bool isPaused = false;

        /// Rate limits renders
        RateLimiter renderLimiter{ 1.0f / 240.0f };

        /// Root node. Child nodes add components that implement specific behaviors for each
        /// node
        SP<WorldNode> root = MAKE<WorldNode>("Root");

        /// If true, we are using a list of node pointers, so removes are locked
        bool isRemoveNodesLocked{};

    public:
        /// Window that corresponds to this world
        SomePlatformWindow* window{};

        /// Stores objects that need time updates
        Updatables updatables;

        /// Use to display render stats like fps, render count, etc.
        Tags renderStats;

        /// Stores resources loaded for this world
        ResourceCatalog resources;

        /// Renders the world
        UP<SomeRenderEngine> renderEngine;

        /// Render context for this world
        UP<SomeRenderContext> renderContext;

        /// Polls for UI events
        UP<SomeUIEventPoller> uiEventPoller;

        /// Design system for quick build UI
        /// Allows us to add theme components with ViewBuilder
        UP<DesignSystem> designSystem;

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

        SomePlatformWindow* Window() const {
            return window;
        }

        Vector2Int PixelSize() const;

        /// @return Returns true if removing nodes is not allowed
        bool IsRemoveNodesLocked() const {
            return isRemoveNodesLocked;
        }

        /// Sets the limited render rate in frames per second
        void SetRenderRate(float value) {
            renderLimiter.core.rate = 1.0f / value;
        }

        /// Enables or disables limiting the render rate (enabled by default)
        /// Limit render rates to avoid wasting battery life on portable devices
        void SetIsRenderRateLimited(bool value) {
            isRenderRateLimited = value;
        }

        /// @return Returns the first system that matches the type
        template <class Type>
        Type* TypeSystem() {
            auto found = std::find_if(systems.begin(), systems.end(), [](auto& system) {
                return dynamic_cast<Type*>(system.get()) != nullptr;
            });

            GUARDR(found != systems.end(), nullptr)
            return dynamic_cast<Type*>(found->get());
        }

        /// @return Returns the first system that matches the type
        /// Convenience name
        template <class Type>
        Type* GetSystem() {
            return TypeSystem<Type>();
        }

        /// @return Returns the main camera for this world. This is the camera used to render
        /// the window's content
        virtual SomeCamera* MainCamera() const;

        /// @return Returns a matrix that transforms this node's vertices in local space (position +
        /// offset + scale + rotation)
        virtual Matrix4x4 LocalModelMatrix(WorldNode const& node);

        /// @return Returns a matrix that transforms this node's vertices into world space
        virtual Matrix4x4 WorldModelMatrix(WorldNode const& node);

        /// Sends update event to world
        FinishType OnUpdate(TimeSlice time);

        /// Renders if the rate limiter allows it
        virtual void Render();

        /// Renders immediately
        virtual void RenderNow();

        /// Adds a node to the root node
        void Add(SP<WorldNode> const& node);

        /// Adds a system
        void Add(SP<SomeWorldSystem> const& system);

        /// Adds a node with arguments
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
        SystemList& Systems() {
            return systems;
        }

        /// @return Returns the list of systems attached to this world
        SystemList const& Systems() const {
            return systems;
        }

        /// @return Returns the screen position for the world position
        ScreenPosition WorldToScreen(WorldPosition worldPos) const;

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
        void Remove(VectorList<SomeWorldSystem*> const& systems);

        /// Removes all systems
        void RemoveAllSystems();

        /// Remove all nodes except for the root node
        /// Warning: do not call this for immediate UI
        void RemoveAllNodes();

        /// Mark all nodes to be destroyed
        /// Safe to call for immediate UI
        void DestroyAllNodes();

        /// Called to process UI events
        virtual void ProcessUIEvents(UIEventList const& uiEvents);

    protected:
        // MARK: Base

        void OnGo() override;
    };

    /// @return Returns the local position in this component's owner space for the screen position
    LocalPosition ScreenToLocal(SomeWorldComponent const& component, ScreenPosition screenPos);

    /// @return Returns the world positionin this component's owner space for the screen position
    WorldPosition ScreenToWorld(SomeWorldComponent const& component, ScreenPosition screenPos);

    /// @return Returns the screen position for the local position
    ScreenPosition LocalToScreen(SomeWorldComponent const& component, LocalPosition localPos = {});

    /// @return Returns the screen position for the world position
    ScreenPosition WorldToScreen(SomeWorldComponent const& component, WorldPosition worldPos);

    /// @return Returns the screen position for the node's world position
    ScreenPosition WorldToScreen(WorldNode const& node);
} // namespace PJ
