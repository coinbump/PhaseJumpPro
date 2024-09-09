#ifndef PJWORLD_H
#define PJWORLD_H

#include "EventWorldSystem.h"
#include "LoadedResources.h"
#include "OrthoCamera.h"
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
    class SomeTexture;
    class DesignSystem;

    /// Defines properties and methods for building a UI
    class DesignSystem {
    protected:
        /// Maps UI element ids to specific textures
        UnorderedMap<String, SP<SomeTexture>> elementTextures;

        /// Maps UI element ids to custom properties
        UnorderedMap<String, Tags> elementTags;

    public:
        void Add(String element, SP<SomeTexture> texture) {
            elementTextures.insert_or_assign(element, texture);
        }

        template <class T>
        void SetTag(String element, String tagName, T value) {
            elementTags[element].Add(tagName, value);
        }

        template <class T>
        T TagValue(String element, String tagName) const {
            try {
                auto& tags = elementTags.at(element);
                return tags.SafeValue<T>(tagName);
            } catch (...) {
                return T();
            }
        }

        SP<SomeTexture> Texture(String element) const {
            try {
                return elementTextures.at(element);
            } catch (...) {}

            return nullptr;
        }
    };

    /// Builds a UI using only meshes, no textures
    class MeshDesignSystem {
        // FUTURE: implement as needed
    };

    /// Simple, basic design system ("ugly duckling" UI)
    class DuckDesignSystem {
        // FUTURE: implement as needed
    };

    // TODO: Needs unit tests
    class World : public Base, public Updatable {
    protected:
        using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;

        std::optional<TimePoint> fpsCheckTimePoint;
        uint64_t renderFrameCount;

        void GoInternal() override;

        WorldNode::NodeList updateList;
        VectorList<SP<SomeWorldSystem>> systems;
        SP<SomeCamera> mainCamera;

        bool isPaused = false;

    public:
        /// Use to display render stats like fps, render count, etc.
        Tags renderStats;

        SP<WorldNode> root = MAKE<WorldNode>("Root");
        SP<LoadedResources> loadedResources = MAKE<LoadedResources>();
        SP<SomeRenderContext> renderContext;
        SP<SomeCamera> camera;
        SP<SomeUIEventPoller> uiEventPoller;

        /// (Optional). Design system for quick build UI
        SP<DesignSystem> designSystem;

        /// Render materials that can be shared between objects
        UnorderedMap<String, SP<RenderMaterial>> renderMaterials;

        World();

        /// Returns the first system that matches the type
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
        virtual void Render();

        void Add(SP<WorldNode> node);
        void Add(SP<SomeWorldSystem> system);

        template <typename... Arguments>
        WorldNode& AddNode(Arguments... args) {
            SP<WorldNode> result = MAKE<WorldNode>(args...);
            Add(result);
            return *result;
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

        void Remove(SP<SomeWorldSystem> system);
        void Remove(VectorList<SP<SomeWorldSystem>> systems);
        void RemoveAllSystems();
        void RemoveAllNodes();

        void SetRenderContext(SP<SomeRenderContext> renderContext);

        virtual void ProcessUIEvents(List<SP<SomeUIEvent>> const& uiEvents);

        // MARK: Quick build

        SP<SomeTexture> Texture(String name);
    };

    LocalPosition ScreenToLocal(SomeWorldComponent& component, ScreenPosition screenPos);
} // namespace PJ

#endif
