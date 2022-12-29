#ifndef PJWORLD_H
#define PJWORLD_H

#include "WorldNode.h"
#include "Updatable.h"
#include "SomeRenderContext.h"
#include <memory>

namespace PJ
{
    class World : public Base, public Updatable {
    public:
        std::shared_ptr<WorldNode> root = std::make_shared<WorldNode>();
        std::shared_ptr<SomeRenderContext> renderContext;

        bool IsFinished() const override { return false; }
        void OnUpdate(TimeSlice time) override
        {
            root->OnUpdate(time);

            auto graph = root->CollectGraph();
            for (auto node : graph) {
                auto worldNode = std::dynamic_pointer_cast<WorldNode>(node);
                if (worldNode->IsDestroyed()) {
                    auto parent = worldNode->Parent();
                    if (parent) {
                        parent->RemoveEdgesTo(worldNode);
                    }
                }
            }
        }
    };
}

#endif
