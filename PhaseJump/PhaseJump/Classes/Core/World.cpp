#include "World.h"
#include "DesignSystem.h"
#include "DevProfiler.h"
#include "DuckDesignSystem.h"
#include "Font.h"
#include "Funcs.h"
#include "GraphNodeTool.h"
#include "RenderContextModel.h"
#include "SomeCamera.h"
#include "SomePlatformWindow.h"
#include "SomeRenderEngine.h"
#include "SomeRenderer.h"
#include "SomeShaderProgram.h"
#include <stdio.h>

// #define PROFILE

using namespace std;
using namespace PJ;

void BuildUpdateList(WorldNode& node, VectorList<WorldNode*>& updateList) {
    if (node.IsDestroyed()) {
        node.OnDestroy();

        // FUTURE: send OnDestroy to children if needed (ineffecient)

        GUARD(node.Parent())

        // TODO: should we be removing and looping at the same time? (ok for now with SP copy, but
        // what if that changes?)
        node.Parent()->Remove(node);
    } else {
        GUARD(node.IsEnabled() && node.World())

        updateList.push_back(&node);

        auto childNodes = node.ChildNodes();
        std::for_each(childNodes.begin(), childNodes.end(), [&](auto& node) {
            BuildUpdateList(*node, updateList);
        });
    }
}

World::World() {
    root->world = this;

    // The render limiter has to keep running while the world is paused, or imGui stops working
    // Wrong: updatables.Add(renderLimiter);

    // Default design system is the "ugly duckling" design system,
    // a basic and simple UI
    designSystem = MAKE<DuckDesignSystem>();
}

void World::Add(SP<SomeWorldSystem> system) {
    system->SetWorld(this);
    systems.push_back(system);
}

void World::RemoveAllNodes() {
    GUARD(root)

    if (isRemoveNodesLocked) {
        // If this happens, use Destroy instead
        PJ::Log("ERROR. Can't remove nodes");
        return;
    }

    /*
     Warning: do not call this for immediate UI

     Warning: This leaves dangling pointers to world, but we assume
     that RemoveAll causes all child objects to be de-allocated
     */
    root->RemoveAllChildren();
}

void World::DestroyAllNodes() {
    GUARD(root)

    // Mark nodes to be destroyed when it is safe to do so
    for (auto& child : root->Children()) {
        child->Destroy();
    }
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

SomeCamera* World::MainCamera() const {
#ifdef PROFILE
    DevProfiler devProfiler("MainCamera", [](String value) { cout << value; });
#endif

    if (!mainCamera.expired()) {
        auto cameraResult = mainCamera.lock().get();

        // Even if we have a reference to a camera, make sure it is valid
        if (cameraResult->owner && cameraResult->owner->World()) {
            return cameraResult;
        }
    }

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
    std::cout << "PHASE JUMP IS DEBUG" << std::endl;
#endif

    RateLimiter::OnFireFunc overrideFunc = [this](auto& limiter) { RenderNow(); };
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

    VectorList<WorldNode*> updateList;
    BuildUpdateList(*root, updateList);

    isRemoveNodesLocked = true;
    Defer defer([&]() { isRemoveNodesLocked = false; });

    VectorList<SomeCamera*> optionalCameras;
    std::transform(
        updateList.begin(), updateList.end(), std::back_inserter(optionalCameras),
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

    RenderContextModel contextModel{ .renderContext = renderContext.get(),
                                     .root = root.get(),
                                     .nodes = updateList,
                                     .cameras = cameras };

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

FinishType World::OnUpdate(TimeSlice _time) {
    VectorList<WorldNode*> updateList;
    BuildUpdateList(*root, updateList);

    isRemoveNodesLocked = true;
    Defer defer([&]() { isRemoveNodesLocked = false; });

#ifdef PROFILE
    DevProfiler devProfiler("OnUpdate", [](String value) { cout << value; });
#endif

    // Keep the renderer running even while paused
    // This allows immediate renders to work (Example: imGUI)
    renderLimiter.OnUpdate(_time);

    GUARDR(!isPaused, FinishType::Continue)

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

    for (auto& node : updateList) {
        node->CheckedAwake();
    }

    for (auto& node : updateList) {
        node->CheckedStart();
    }

    for (auto& node : updateList) {
        node->OnUpdate(time);
    }

    for (auto& node : updateList) {
        node->LateUpdate();
    }

    return FinishType::Continue;
}

void World::Add(SP<WorldNode> node) {
#ifdef PROFILE
    DevProfiler devProfiler("Add", [](String value) { cout << value; });
#endif

    auto parent = root;
    GUARD(node && parent)

    parent->Add(node);
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

Vector2Int World::PixelSize() const {
    GUARDR(window, {})
    return window->PixelSize();
}

#include "Matrix4x4.h"
#include "World.h"
#include "WorldNode.h"
#include <TSMatrix4D.h>

using namespace PJ;

ScreenPosition World::WorldToScreen(WorldPosition worldPos) const {
    auto camera = MainCamera();
    GUARDR(camera, {})

    // TODO: this isn't taking into account the hiDp world scale
    auto result = camera->WorldToScreen(worldPos);
    return result;
}

WorldPosition PJ::ScreenToWorld(SomeWorldComponent const& component, ScreenPosition screenPos) {
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

LocalPosition PJ::ScreenToLocal(SomeWorldComponent const& component, ScreenPosition screenPos) {
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

ScreenPosition PJ::LocalToScreen(SomeWorldComponent const& component, LocalPosition localPos) {
    return WorldToScreen(component, component.LocalToWorld(localPos));
}

ScreenPosition PJ::WorldToScreen(SomeWorldComponent const& component, WorldPosition worldPos) {
    auto owner = component.owner;
    GUARDR(owner, {})

    auto world = owner->World();
    GUARDR(world, {})

    return world->WorldToScreen(worldPos);
}

ScreenPosition PJ::WorldToScreen(WorldNode const& node) {
    auto world = node.World();
    GUARDR(world, {})

    return world->WorldToScreen(node.transform.WorldPosition());
}
