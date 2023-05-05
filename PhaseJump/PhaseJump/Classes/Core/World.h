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

        bool IsFinished() const override { return false; }
        void OnUpdate(TimeSlice time) override
        {
            updateGraph = root->CollectBreadthFirstGraph();

            auto iterSystems = systems;
            for (auto system : iterSystems) {
                system->OnUpdate(time);
            }

            for (auto node : updateGraph) {
                auto worldNode = DCAST<WorldNode>(node);
                if (worldNode->IsDestroyed()) {
                    worldNode->OnDestroy();

                    auto subgraph = worldNode->CollectGraph();
                    for (auto node : subgraph) {
                        auto worldNode = SCAST<WorldNode>(node);
                        worldNode->Destroy();
                    }

                    auto parent = worldNode->Parent();
                    if (parent) {
                        parent->RemoveEdgesTo(worldNode);
                    }
                } else if (worldNode->IsActive()) {
                    worldNode->OnUpdate(time);
                }
            }
        }

        virtual void Render();

        void Add(SP<WorldNode> node) {
            root->AddEdge(StandardEdgeModel(), node);
        }

        void SetRenderContext(SP<SomeRenderContext> renderContext) {
            this->renderContext = renderContext;
        }

        virtual void ProcessUIEvents(VectorList<SP<SomeUIEvent>> const& uiEvents);
    };
}

#endif
