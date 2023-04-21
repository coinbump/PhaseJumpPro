#ifndef PJWORLD_H
#define PJWORLD_H

#include "WorldNode.h"
#include "WorldComponent.h"
#include "SomeUIEventPoller.h"
#include "Updatable.h"
#include "SomeRenderContext.h"
#include "OrthoCamera.h"
#include <memory>

namespace PJ
{
    class World : public Base, public Updatable {
    public:
        std::shared_ptr<WorldNode> root = std::make_shared<WorldNode>();
        std::shared_ptr<SomeRenderContext> renderContext;
        std::shared_ptr<SomeCamera> camera;
        std::shared_ptr<SomeUIEventPoller> uiEventPoller;

        World()
        {
            camera = std::static_pointer_cast<SomeCamera>(std::make_shared<OrthoCamera>());

            auto cameraNode = std::make_shared<WorldNode>();
            cameraNode->Add(camera);
            root->AddEdge(StandardEdgeModel(), cameraNode);
        }

        bool IsFinished() const override { return false; }
        void OnUpdate(TimeSlice time) override
        {
            auto graph = root->CollectGraph();
            for (auto node : graph) {
                auto worldNode = std::dynamic_pointer_cast<WorldNode>(node);
                if (worldNode->IsDestroyed()) {
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

        void Add(std::shared_ptr<WorldNode> node) {
            root->AddEdge(StandardEdgeModel(), node);
        }

        void SetRenderContext(std::shared_ptr<SomeRenderContext> renderContext) {
            this->renderContext = renderContext;
        }
    };
}

#endif
