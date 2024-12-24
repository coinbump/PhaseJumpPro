#include "World.h"
#include "DesignSystem.h"
#include "DevProfiler.h"
#include "DuckDesignSystem.h"
#include "Font.h"
#include "Funcs.h"
#include "GraphNodeTool.h"
#include "RenderContextModel.h"
#include "SomeCamera.h"
#include "SomeRenderEngine.h"
#include "SomeRenderer.h"
#include "SomeShaderProgram.h"
#include <stdio.h>

// #define PROFILE

using namespace std;
using namespace PJ;

World::World() {
    root->world = this;

    // The render limiter has to keep running while the world is paused, or imGui stops working
    // Wrong: updatables.Add(renderLimiter);

    // Default design system is "ugly duckling" design system,
    // a basic and simple UI
    designSystem = MAKE<DuckDesignSystem>();
}

void World::Add(SP<SomeWorldSystem> system) {
    system->SetWorld(this);
    systems.push_back(system);
}

void World::RemoveAllNodes() {
    GUARD(root)

    // This leaves dangling pointers to world, but we assume
    // that RemoveAll causes all child objects to be de-allocated
    root->RemoveAllChildren();
}

void World::RemoveAllSystems() {
    Remove(systems);
}

void World::Remove(SomeWorldSystem& system) {
    GUARD(system.World() == this)

    system.SetWorld({});
    RemoveFirstIf(systems, [&](auto& i) { return i.get() == &system; });
}

// List is passed as copy to prevent iteration-mutation crash
void World::Remove(VectorList<SP<SomeWorldSystem>> systems) {
    for (auto& system : systems) {
        Remove(*system);
    }
}

SomeCamera* World::MainCamera() {
#ifdef PROFILE
    DevProfiler devProfiler("MainCamera", [](String value) { cout << value; });
#endif

    GUARDR(mainCamera.expired(), mainCamera.lock().get())

    VectorList<WorldNode*> graph;
    CollectBreadthFirstTree(root.get(), graph);

    for (auto& node : graph) {
        auto worldNode = node;
        auto camera = worldNode->GetComponent<SomeCamera>();

        // Offscreen cameras don't qualify as the main camera
        GUARD_CONTINUE(camera && !camera->renderContext)

        auto sharedCamera = camera ? SCAST<SomeCamera>(camera->shared_from_this()) : nullptr;
        mainCamera = sharedCamera;
        return sharedCamera.get();
    }

    return {};
}

void World::OnGo() {
    Base::OnGo();

#ifdef DEBUG
    std::cout << "PHASE JUMP IS DEBUG";
#endif

    SomeLimiter::OnFireFunc overrideFunc = [this](auto& limiter) { RenderNow(); };
    Override(renderLimiter.onFireFunc, overrideFunc);
}

void World::Render() {
    if (isRenderRateLimited) {
        renderLimiter.Fire();
    } else {
        RenderNow();
    }
}

void World::RenderNow() {
    GUARD(renderContext)

#ifdef PROFILE
    DevProfiler devProfiler("Render", [](String value) { cout << value; });
#endif
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

    // TODO: should we use updateList?
    VectorList<WorldNode*> nodes;
    nodes.reserve(updateList.size());
    std::transform(
        updateList.begin(), updateList.end(), std::back_inserter(nodes),
        [](SP<WorldNode> node) { return node.get(); }
    );

    VectorList<SomeCamera*> optionalCameras;
    std::transform(
        nodes.begin(), nodes.end(), std::back_inserter(optionalCameras),
        [](WorldNode* node) {
            auto result = node->GetComponent<SomeCamera>();
            return (result && result->IsEnabled()) ? result : nullptr;
        }
    );
    VectorList<SomeCamera*> cameras;
    std::copy_if(
        optionalCameras.begin(), optionalCameras.end(), std::back_inserter(cameras),
        [](SomeCamera* camera) { return camera != nullptr; }
    );

    RenderContextModel contextModel{
        .renderContext = renderContext.get(), .root = root.get(), .nodes = nodes, .cameras = cameras
    };

    int drawCount = 0;

    // Prevent iterate-mutation crash
    auto iterSystems = systems;
    for (auto& system : iterSystems) {
        auto result = system->Render(contextModel);

        if (result) {
            drawCount += result->tags.SafeValue<int>(RenderStatId::DrawCount);
        }
    }

    renderStats.Set(RenderStatId::DrawCount, drawCount);
    renderStats.Set(RenderStatId::NodeCount, (int)contextModel.nodes.size());

    renderFrameCount++;
}

void World::ProcessUIEvents(UIEventList const& uiEvents) {
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
    translateMatrix.LoadTranslate(node.transform.LocalPosition() + node.transform.Value().offset);
    scaleMatrix.LoadScale(node.transform.Scale());

    // This is 2D rotation only
    // FUTURE: support 3D rotation if needed.
    rotationMatrix.LoadZRadRotation(Angle::WithDegrees(node.transform.Rotation().z).Radians());

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

// TODO: come back to this
void UpdateWorld(WorldNode& node, TimeSlice time, WorldNode::NodeList& updateList) {
    if (node.IsDestroyed()) {
        node.OnDestroy();

        // TODO: send OnDestroy to children
        GUARD(node.Parent())
        node.Parent()->Remove(node);
    } else {
        GUARD(node.IsEnabled())
        // TODO: rethink. Does update need shared points? What if something is removed?
        updateList.push_back(SCAST<WorldNode>(node.shared_from_this()));

        if (!node.World()->IsPaused()) {
            // TODO: this isn't correct. All nodes should get CheckedAwake, then all nodes should
            // get CheckedStart
            node.CheckedAwake();
            node.CheckedStart();

            node.OnUpdate(time);
        }

        auto childNodes = node.ChildNodes();
        std::for_each(childNodes.begin(), childNodes.end(), [&](SP<WorldNode>& node) {
            UpdateWorld(*node, time, updateList);
        });
    }
}

void World::OnUpdate(TimeSlice _time) {
#ifdef PROFILE
    DevProfiler devProfiler("OnUpdate", [](String value) { cout << value; });
#endif

    // Keep the renderer running even while paused
    // This allows immediate renders to work (Example: imGUI)
    renderLimiter.OnUpdate(_time);

    GUARD(!isPaused)

    auto time = _time * timeScale;

    updatables.OnUpdate(time);

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
    UpdateWorld(*root, time, updateList);
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

void World::LoadPrefab(String id, WorldNode& node) {
    GUARD_LOG(node.World() == this, "ERROR. Node must be parented before LoadPrefab")

    try {
        auto prefab = prefabs.at(id);
        prefab->LoadInto(node);
    } catch (...) {}
}

bool World::IsPaused() const {
    return isPaused;
}

void World::Pause() {
    isPaused = true;
}

void World::Play() {
    isPaused = false;
}

#include "Matrix4x4.h"
#include "World.h"
#include "WorldNode.h"
#include <TSMatrix4D.h>

using namespace PJ;

WorldPosition PJ::ScreenToWorld(SomeWorldComponent& component, ScreenPosition screenPos) {
    auto owner = component.owner;
    GUARDR(owner, {})

    auto world = owner->World();
    GUARDR(world, {})

    // Get the camera
    auto camera = world->MainCamera();
    GUARDR(camera, {})

    auto result = camera->ScreenToWorld(screenPos);
    return result;
}

LocalPosition PJ::ScreenToLocal(SomeWorldComponent& component, ScreenPosition screenPos) {
    LocalPosition result;

    auto owner = component.owner;
    GUARDR(owner, result)

    auto world = owner->World();
    GUARDR(world, result)

    // Get the camera
    auto camera = world->MainCamera();
    GUARDR(camera, result)

    auto worldPosition = camera->ScreenToWorld(screenPos);

    auto worldModelMatrix = world->WorldModelMatrix(*owner);
    Terathon::Point3D point(worldPosition.x, worldPosition.y, worldPosition.z);
    auto localPosition = Terathon::InverseTransform(worldModelMatrix, point);

    result = LocalPosition(localPosition.x, localPosition.y, 0);

    return result;
}
