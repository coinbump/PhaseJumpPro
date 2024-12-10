#pragma once

#include "EventWorldSystem.h"
#include "OrthoCamera.h"
#include "Prefab.h"
#include "RateLimiter.h"
#include "ResourceModels.h"
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

    // TODO: Needs unit tests
    // TODO: re-evaluate use of multiple inheritance here (doesn't need Updatable)
    class World : public Base, public Updatable {
    protected:
        using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;

        std::optional<TimePoint> fpsCheckTimePoint;
        uint64_t renderFrameCount;

        // MARK: Base

        void OnGo() override;

        WorldNode::NodeList updateList;
        VectorList<SP<SomeWorldSystem>> systems;
        SP<SomeCamera> mainCamera;

        bool isPaused = false;

        /// Rate limits renders
        RateLimiter renderLimiter{ 1.0f / 240.0f };

    public:
        /// Number of pixels per point
        /// Example: on a Retina screen, each point represents 2 pixels, this value would be 2
        float uiScale = 1;

        /// Stores objects that need time updates
        Updatables updatables;

        /// Use to display render stats like fps, render count, etc.
        Tags renderStats;

        SP<WorldNode> root = MAKE<WorldNode>("Root");
        SP<ResourceModels> loadedResources = MAKE<ResourceModels>();
        SP<SomeRenderContext> renderContext;
        SP<SomeCamera> camera;

        // TODO: UP here
        SP<SomeUIEventPoller> uiEventPoller;

        /// (Optional). Design system for quick build UI
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

        virtual SP<SomeCamera> MainCamera();

        virtual Matrix4x4 LocalModelMatrix(WorldNode const& node);
        virtual Matrix4x4 WorldModelMatrix(WorldNode const& node);

        void OnUpdate(TimeSlice time) override;

        /// Rate-limited render
        virtual void Render();

        /// Render immediately
        virtual void RenderNow();

        void Add(SP<WorldNode> node);
        void Add(SP<SomeWorldSystem> system);

        template <typename... Arguments>
        WorldNode& AddNode(Arguments... args) {
            SP<WorldNode> result = MAKE<WorldNode>(args...);
            Add(result);
            return *result;
        }

        template <typename... Arguments>
        constexpr WorldNode& And(Arguments... args) {
            return AddNode(args...);
        }

        template <typename... Arguments>
        WorldNode& AddNodeAt(Vector3 pos, Arguments... args) {
            SP<WorldNode> result = MAKE<WorldNode>(args...);
            Add(result);
            result->SetLocalPosition(pos);
            return *result;
        }

        VectorList<SP<SomeWorldSystem>>& Systems() {
            return systems;
        }

        VectorList<SP<SomeWorldSystem>> const& Systems() const {
            return systems;
        }

        //        template <class T>
        //        void AddComponent(SP<T> component) {
        //            auto node = MAKE<WorldNode>();
        //            node->AddComponent(component);
        //            Add(node);
        //        }

        bool IsPaused() const {
            return isPaused;
        }

        void Pause() {
            isPaused = true;
        }

        void Play() {
            isPaused = false;
        }

        void LoadPrefab(String id, WorldNode& node) {
            GUARD_LOG(node.World() == this, "ERROR. Node must be parented before LoadPrefab")

            try {
                auto prefab = prefabs.at(id);
                prefab->LoadInto(node);
            } catch (...) {}
        }

        void Remove(SomeWorldSystem& system);
        void Remove(VectorList<SP<SomeWorldSystem>> systems);
        void RemoveAllSystems();
        void RemoveAllNodes();

        void SetRenderContext(SP<SomeRenderContext> renderContext);

        virtual void ProcessUIEvents(UIEventList const& uiEvents);

        SP<SomeTexture> FindTexture(String id);
        UnorderedSet<SP<SomeTexture>> FindTextures(UnorderedSet<String> const& textureIds);
        VectorList<SP<SomeTexture>> FindTextureList(VectorList<String> const& textureIds);
        SP<Font> FindFont(FontSpec spec);
        SP<Font> FindFontWithSize(float size);
        SP<Font> FindFontWithResourceId(String id);
        SP<SomeShaderProgram> FindShader(String id);
    };

    LocalPosition ScreenToLocal(SomeWorldComponent& component, ScreenPosition screenPos);
    WorldPosition ScreenToWorld(SomeWorldComponent& component, ScreenPosition screenPos);

} // namespace PJ
