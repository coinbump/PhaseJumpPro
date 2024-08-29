#ifndef PJWORLD_H
#define PJWORLD_H

#include "EventSystem.h"
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

    public:
        /// Use to display render stats like fps, render count, etc.
        Tags renderStats;

        SP<WorldNode> root = MAKE<WorldNode>("Root");
        SP<LoadedResources> loadedResources = MAKE<LoadedResources>();
        SP<SomeRenderContext> renderContext;
        SP<SomeCamera> camera;
        SP<SomeUIEventPoller> uiEventPoller;

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

        void Remove(SP<SomeWorldSystem> system);
        void Remove(VectorList<SP<SomeWorldSystem>> systems);
        void RemoveAllSystems();
        void RemoveAllNodes();

        void SetRenderContext(SP<SomeRenderContext> renderContext);

        virtual void ProcessUIEvents(List<SP<SomeUIEvent>> const& uiEvents);
    };

    LocalPosition ScreenToLocal(SomeWorldComponent& component, ScreenPosition screenPos);
} // namespace PJ

#endif
