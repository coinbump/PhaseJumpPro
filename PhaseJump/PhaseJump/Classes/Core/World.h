#ifndef PJWORLD_H
#define PJWORLD_H

#include "WorldComponent.h"
#include "SomeUIEventPoller.h"
#include "Updatable.h"
#include "SomeRenderContext.h"
#include "OrthoCamera.h"
#include "EventSystem.h"
#include "LoadedResources.h"
#include "Macros.h"
#include <memory>

namespace PJ
{
    class WorldNode;
    class WorldSystem;

    class World : public Base, public Updatable {
    protected:
        using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;

        std::optional<TimePoint> fpsCheckTimePoint;
        int renderFrameCount;

        void GoInternal() override;

        WorldNode::NodeVectorList updateGraph;
        VectorList<SP<WorldSystem>> systems;

    public:
        SP<WorldNode> root = MAKE<WorldNode>();
        SP<LoadedResources> loadedResources = MAKE<LoadedResources>();
        SP<SomeRenderContext> renderContext;
        SP<SomeCamera> camera;
        SP<SomeUIEventPoller> uiEventPoller;

        WorldNode::NodeVectorList const& UpdateGraph() const { return updateGraph; }

        World();

        virtual Matrix4x4 LocalModelMatrix(WorldNode const& node);
        virtual Matrix4x4 WorldModelMatrix(WorldNode const& node);

        bool IsFinished() const override { return false; }
        void OnUpdate(TimeSlice time) override;
        virtual void Render();

        void Add(SP<WorldNode> node);

        template <class T>
        void AddComponent(SP<T> component) {
            auto node = MAKE<WorldNode>();
            node->AddComponent(component);
            Add(node);
        }

        void SetRenderContext(SP<SomeRenderContext> renderContext);

        virtual void ProcessUIEvents(VectorList<SP<SomeUIEvent>> const& uiEvents);
    };
}

#endif
