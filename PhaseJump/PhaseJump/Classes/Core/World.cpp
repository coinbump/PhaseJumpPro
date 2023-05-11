#include "World.h"
#include "SomeRenderer.h"
#include "RenderIntoModel.h"
#include "Camera.h"
#include "SomeCamera.h"
#include "SomeRenderEngine.h"
#include <stdio.h>

using namespace std;
using namespace PJ;

World::World()
{
    camera = SCAST<SomeCamera>(MAKE<OrthoCamera>());

    auto cameraNode = MAKE<WorldNode>();
    cameraNode->Add(camera);
    root->AddEdge(StandardEdgeModel(), cameraNode);
}

void World::GoInternal() {
    Base::GoInternal();

    // Can't use shared_from_this in constructor
    root->world = SCAST<World>(this->shared_from_this());

    auto graph = root->CollectBreadthFirstGraph();
    for (auto node : graph) {
        auto worldNode = SCAST<WorldNode>(node);

        // Systems are assumed to be permanent for now
        auto systems = worldNode->GetComponents<WorldSystem>();
        for (auto system : systems) {
            this->systems.Add(system);
        }

        if (nullptr == Camera::main) {
            auto camera = worldNode->GetComponent<SomeCamera>();
            if (camera) {
                Camera::main = camera;
            }
        }
    }

    // All nodes get Awake first, then all get Start
    for (auto node : graph) {
        auto worldNode = SCAST<WorldNode>(node);
        worldNode->Awake();
    }

    for (auto node : graph) {
        auto worldNode = SCAST<WorldNode>(node);
        worldNode->Start();
    }
}

void World::Render()
{
    if (!renderContext) { return; }

    renderContext->Bind();
    renderContext->Clear();

#ifdef _FPS_CHECK_
    auto now = std::chrono::steady_clock::now();
    if (fpsCheckTimePoint) {
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - fpsCheckTimePoint.value());
        float durationCheck = 1.0f;
        float durationCount = duration.count();

        if (durationCount >= durationCheck) {
            PJLog("World FPS: %d", (int)((float)renderFrameCount / durationCount));
            renderFrameCount = 0;
            fpsCheckTimePoint = now;
        }
    } else {
        fpsCheckTimePoint = now;
    }
#endif

    auto graph = root->CollectBreadthFirstGraph();
    VectorList<SP<WorldNode>> nodes;
    VectorList<SP<WorldNode>> activeNodes;

    std::transform(graph.begin(), graph.end(), std::back_inserter(nodes), [](SP<SomeGraphNode<>> node) {
        return SCAST<WorldNode>(node);
    });

    std::copy_if(nodes.begin(), nodes.end(), std::back_inserter(activeNodes), [](SP<WorldNode> node) {
        return node->IsActive();
    });

    auto renderEngine = renderContext->renderEngine;
    renderEngine->RenderStart();

    for (auto node : activeNodes) {
        auto camera = node->GetComponent<SomeCamera>();
        if (nullptr == camera) {
            continue;
        }

        camera->Render(activeNodes, renderContext);
    }

    renderEngine->RenderDraw();

    renderContext->Present();

    renderFrameCount++;
}

void World::ProcessUIEvents(VectorList<SP<SomeUIEvent>> const& uiEvents) {
    auto iterSystems = systems;
    for (auto system : iterSystems) {
        system->ProcessUIEvents(uiEvents);
    }
}

Matrix4x4 World::LocalModelMatrix(WorldNode const& node) {
    Matrix4x4 translateMatrix, scaleMatrix, rotationMatrix;
    translateMatrix.LoadTranslate(node.transform->LocalPosition());
    scaleMatrix.LoadScale(node.transform->Scale());

    // This is 2D rotation only
    // FUTURE: support 3D rotation if needed.
    rotationMatrix.LoadZRadRotation(Angle::DegreesAngle(node.transform->Rotation().z).Radians());

    auto m1 = translateMatrix * rotationMatrix;
    auto modelMatrix = m1 * scaleMatrix;
    return modelMatrix;
}

Matrix4x4 World::WorldModelMatrix(WorldNode const& node) {
    auto modelMatrix = LocalModelMatrix(node);

    auto parent = SCAST<WorldNode>(node.Parent());

    // Transform child to parent matrix
    while (parent) {
        auto parentModelMatrix = LocalModelMatrix(*parent);

        modelMatrix = parentModelMatrix * modelMatrix;

        parent = SCAST<WorldNode>(parent->Parent());
    }

    Matrix4x4 worldScaleMatrix;
    worldScaleMatrix.LoadIdentity();

    modelMatrix = worldScaleMatrix * modelMatrix;

    return modelMatrix;
}

void World::OnUpdate(TimeSlice time)
{
    updateGraph = root->CollectBreadthFirstGraph();

    auto iterSystems = systems;
    for (auto system : iterSystems) {
        system->OnUpdate(time);
    }

    VectorList<SP<WorldNode>> wakeupList;
    for (auto node : updateGraph) {
        auto worldNode = SCAST<WorldNode>(node);

        if (!worldNode->IsAwake()) {
            worldNode->Awake();
            wakeupList.Add(worldNode);
        }
    }

    for (auto worldNode : wakeupList) {
        if (!worldNode->IsStarted()) {
            worldNode->Start();
        }
    }

    for (auto node : updateGraph) {
        auto worldNode = SCAST<WorldNode>(node);

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

void World::Add(SP<WorldNode> node) {
    root->AddEdge(StandardEdgeModel(), node);
}

void World::SetRenderContext(SP<SomeRenderContext> renderContext) {
    this->renderContext = renderContext;
}
