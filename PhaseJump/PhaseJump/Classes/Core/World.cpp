#include "World.h"
#include "Camera.h"
#include "DevProfiler.h"
#include "GraphNodeTool.h"
#include "RenderIntoModel.h"
#include "SomeCamera.h"
#include "SomeRenderEngine.h"
#include "SomeRenderer.h"
#include <stdio.h>

// #define PROFILE

using namespace std;
using namespace PJ;

World::World() {
    root->world = this;
}

void World::Add(SP<SomeWorldSystem> system) {
    system->SetWorld(this);
    this->systems.Add(system);
}

void World::RemoveAllNodes() {
    GUARD(root)

    // This leaves dangling pointers to world, but we assume
    // that RemoveAll causes all child objects to be de-allocated
    root->RemoveAllEdges();

    // Clear cached values
    mainCamera.reset();
}

void World::RemoveAllSystems() {
    Remove(systems);
}

void World::Remove(SP<SomeWorldSystem> system) {
    GUARD(system->World() == this)

    system->SetWorld(nullptr);
    PJ::Remove(systems, system);
}

void World::Remove(VectorList<SP<SomeWorldSystem>> systems) {
    for (auto& system : systems) {
        Remove(system);
    }
}

SP<SomeCamera> World::MainCamera() {
#ifdef PROFILE
    DevProfiler devProfiler("MainCamera", [](String value) { cout << value; });
#endif

    GUARDR(!mainCamera, mainCamera)

    GraphNodeTool<> graphNodeTool;
    auto graph = graphNodeTool.CollectBreadthFirstGraph(root);
    for (auto& node : graph) {
        auto worldNode = SCAST<WorldNode>(node);

        auto camera = worldNode->GetComponent<SomeCamera>();
        if (camera) {
            mainCamera = camera;
            return mainCamera;
        }
    }

    return nullptr;
}

void World::GoInternal() {
    Base::GoInternal();

    GraphNodeTool<> graphNodeTool;
    auto graph = graphNodeTool.CollectBreadthFirstGraph(root);

    // All nodes get Awake first, then all get Start
    for (auto& node : graph) {
        auto worldNode = SCAST<WorldNode>(node);
        worldNode->CheckedAwake();
    }

    for (auto& node : graph) {
        auto worldNode = SCAST<WorldNode>(node);
        worldNode->CheckedStart();
    }
}

void World::Render() {
#ifdef PROFILE
    DevProfiler devProfiler("Render", [](String value) { cout << value; });
#endif

    // TODO: should this be a system?
    GUARD(renderContext)

    renderContext->Bind();
    renderContext->Clear();

    int drawCount = 0;

    auto now = std::chrono::steady_clock::now();
    if (fpsCheckTimePoint) {
        auto duration =
            std::chrono::duration_cast<std::chrono::seconds>(now - fpsCheckTimePoint.value());
        float durationCheck = 1.0f;
        float durationCount = duration.count();

        if (durationCount >= durationCheck) {
            renderStats.Insert({ "fps", (int)((float)renderFrameCount / durationCount) });
            renderFrameCount = 0;
            fpsCheckTimePoint = now;
        }
    } else {
        fpsCheckTimePoint = now;
    }

    VectorList<WorldNode*> activeNodes;
    std::transform(
        updateList.begin(), updateList.end(), std::back_inserter(activeNodes),
        [](SP<WorldNode::Node> node) { return static_cast<WorldNode*>(node.get()); }
    );

    // TODO: should GetComponent return a non-smart pointer?
    VectorList<SP<SomeCamera>> activeNullableCameras;
    std::transform(
        activeNodes.begin(), activeNodes.end(), std::back_inserter(activeNullableCameras),
        [](WorldNode* node) { return node->GetComponent<SomeCamera>(); }
    );

    VectorList<SP<SomeCamera>> activeCameras;
    std::copy_if(
        activeNullableCameras.begin(), activeNullableCameras.end(),
        std::back_inserter(activeCameras), [](SP<SomeCamera> camera) { return camera != nullptr; }
    );

    auto renderEngine = renderContext->renderEngine;

    for (auto& camera : activeCameras) {
        RenderContextModel renderContextModel;

        if (camera) {
            camera->PreRender(activeNodes, renderContext, renderContextModel);
        }

        renderEngine->RenderStart(renderContextModel);

        if (camera) {
#ifdef PROFILE
            DevProfiler devProfilerRenderCamera("Render- Camera", [](String value) {
                cout << value;
            });
#endif

            auto cameraRenderModel =
                camera->MakeRenderModel(activeNodes, *renderContext, renderContextModel);

            drawCount += cameraRenderModel.renderModels.size();

            for (auto& renderModel : cameraRenderModel.renderModels) {
#ifdef PROFILE
                DevProfiler devProfilerRenderProcess("Render- Process", [](String value) {
                    cout << value;
                });
#endif
                renderEngine->RenderProcess(renderModel);
            }
        }

        renderEngine->RenderDraw();
    }

    renderStats.Insert("draw.count", drawCount);

    renderContext->Present();

    renderFrameCount++;
}

void World::ProcessUIEvents(List<SP<SomeUIEvent>> const& uiEvents) {
    GUARD(!IsEmpty(uiEvents))

#ifdef PROFILE
    DevProfiler devProfiler("ProcessUIEvents", [](String value) { cout << value; });
#endif

    auto iterSystems = systems;
    for (auto& system : iterSystems) {
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
#ifdef PROFILE
    // DevProfiler devProfiler("WorldModelMatrix", [](String value) { cout << value; });
#endif

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

void Update(WorldNode& node, TimeSlice time, WorldNode::NodeList& updateList) {
    if (node.IsDestroyed()) {
        node.OnDestroy();

        // TODO: send OnDestroy to children
        // TODO: remove the node
        PJLog("WARNING. Destroying nodes is currently broken")
    } else {
        GUARD(node.IsActive())

        node.CheckedAwake();
        node.CheckedStart();

        updateList.push_back(SCAST<WorldNode>(node.shared_from_this()));
        node.OnUpdate(time);

        auto childNodes = node.ChildNodes();
        std::for_each(childNodes.begin(), childNodes.end(), [&](SP<WorldNode>& node) {
            Update(*node, time, updateList);
        });
    }
}

void World::OnUpdate(TimeSlice time) {
#ifdef PROFILE
    DevProfiler devProfiler("OnUpdate", [](String value) { cout << value; });
#endif

    // TODO: is graph being updated twice (when OnUpdate is re-enabled?)

    auto iterSystems = systems;
    for (auto& system : iterSystems) {
        system->CheckedAwake();
    }

    for (auto& system : iterSystems) {
        system->CheckedStart();
    }

    for (auto& system : iterSystems) {
        system->OnUpdate(time);
    }

    updateList.clear();
    Update(*root, time, updateList);
}

void World::Add(SP<WorldNode> node) {
#ifdef PROFILE
    DevProfiler devProfiler("Add", [](String value) { cout << value; });
#endif

    auto parent = root;
    GUARD(node && parent)

    parent->Add(node);
}

void World::SetRenderContext(SP<SomeRenderContext> renderContext) {
    this->renderContext = renderContext;
}
