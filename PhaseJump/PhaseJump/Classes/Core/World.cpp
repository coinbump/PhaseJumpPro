#include "World.h"
#include "Camera.h"
#include "DevProfiler.h"
#include "GraphNodeTool.h"
#include "RenderContextModel.h"
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
    this->systems.push_back(system);
}

void World::RemoveAllNodes() {
    GUARD(root)

    // This leaves dangling pointers to world, but we assume
    // that RemoveAll causes all child objects to be de-allocated
    root->RemoveAllChildren();

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

    WorldNode::NodeList graph;
    CollectBreadthFirstTree(root, graph);

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
        [](SP<WorldNode> node) { return static_cast<WorldNode*>(node.get()); }
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
        RenderContextModel renderContextModel(renderContext.get(), activeNodes);

        renderEngine->RenderStart(renderContextModel);

        if (camera) {
            camera->PreRender(renderContextModel);

#ifdef PROFILE
            DevProfiler devProfilerRenderCamera("Render- Camera", [](String value) {
                cout << value;
            });
#endif

            auto processModel = camera->MakeRenderModel(renderContextModel);

            drawCount += processModel.renderModels.size();

            renderEngine->RenderProcess(processModel);

            renderEngine->RenderDraw();
        }
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
    translateMatrix.LoadTranslate(node.transform.LocalPosition());
    scaleMatrix.LoadScale(node.transform.Scale());

    // This is 2D rotation only
    // FUTURE: support 3D rotation if needed.
    rotationMatrix.LoadZRadRotation(Angle::DegreesAngle(node.transform.Rotation().z).Radians());

    auto m1 = translateMatrix * rotationMatrix;
    auto modelMatrix = m1 * scaleMatrix;
    return modelMatrix;
}

Matrix4x4 World::WorldModelMatrix(WorldNode const& node) {
#ifdef PROFILE
    // DevProfiler devProfiler("WorldModelMatrix", [](String value) { cout << value; });
#endif

    auto modelMatrix = LocalModelMatrix(node);

    auto parent = node.Parent();

    // Transform child to parent matrix
    while (parent) {
        auto parentModelMatrix = LocalModelMatrix(*parent);

        modelMatrix = parentModelMatrix * modelMatrix;

        parent = parent->Parent();
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

        // TODO: this isn't correct. All nodes should get CheckedAwake, then all nodes should get
        // CheckedStart
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
