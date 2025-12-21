#include "World.h"
#include "Camera.h"
#include "DesignSystem.h"
#include "DevProfiler.h"
#include "DuckDesignSystem.h"
#include "Font.h"
#include "Funcs.h"
#include "GraphNodeTool.h"
#include "RenderContextModel.h"
#include "Renderer.h"
#include "ShaderProgram.h"
#include "SomePlatformWindow.h"
#include "SomeRenderEngine.h"
#include <stdio.h>

// #define PROFILE

using namespace std;
using namespace PJ;

void BuildUpdateList(WorldNode& node, VectorList<WorldNode*>& updateList) {
    if (node.IsDestroyed()) {
        node.OnDestroy();

        GUARD(node.Parent())
        node.Parent()->Remove(node);
        return;
    }

    GUARD(node.IsEnabled() && node.World())

    updateList.push_back(&node);

    // Copy shared pointers because we are removing while iterating
    auto childNodes = node.ChildNodes();

    std::for_each(childNodes.begin(), childNodes.end(), [&](auto& node) {
        BuildUpdateList(*node, updateList);
    });
}

// FUTURE: evaluate non-recursive approach (this isn't working correctly: see Views test scene)
// void BuildUpdateList(WorldNode& rootNode, VectorList<WorldNode*>& updateList) {
//    std::stack<WorldNode*> stack;
//    stack.push(&rootNode);
//
//    while (!stack.empty()) {
//        WorldNode* node = stack.top();
//        stack.pop();
//
//        if (node->IsDestroyed()) {
//            node->OnDestroy();
//
//            if (node->Parent()) {
//                node->Parent()->Remove(*node);
//            }
//            continue;
//        }
//
//        if (node->IsEnabled() && node->World()) {
//            updateList.push_back(node);
//
//            auto childNodes = node->ChildNodes();
//
//            for (auto i = childNodes.rbegin(); i != childNodes.rend(); i++) {
//                stack.push((*i).get());
//            }
//        }
//    }
//}

World::World() {
    root->world = this;

    // The render limiter has to keep running while the world is paused, or imGui stops working
    // Wrong: updatables.Add(renderLimiter);

    // Default design system is the "ugly duckling" design system,
    // a basic and simple UI
    designSystem = std::make_unique<DuckDesignSystem>();

    RateLimiter::OnFireFunc overrideFunc = [this](auto& limiter) { RenderNow(); };
    Override(renderLimiter.onFireFunc, overrideFunc);
}

void World::Add(SP<SomeWorldSystem> const& system) {
    GUARD(system)
    GUARD_LOG(system->World() == nullptr, "ERROR. Can't add owned system to world")

    system->SetWorld(this);
    systems.push_back(system);
}

void World::RemoveAllNodes() {
    GUARD(root)

    if (IsRemoveNodesLocked()) {
        /*
         For performance, the render loop takes raw pointers to nodes, so you
         can't remove or alter the node tree while the render is in progress

         Use Destroy, not Remove, which will remove the node when the render loop is finished
         */
        PJ::Log("ERROR. Can't remove nodes");
        return;
    }

    /*
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
    auto mappedSystems = Map<SomeWorldSystem*>(systems, [](auto& system) { return system.get(); });

    Remove(mappedSystems);
}

void World::Remove(SomeWorldSystem& system) {
    GUARD(system.World() == this)

    system.SetWorld({});
    RemoveFirstIf(systems, [&](auto& i) { return i.get() == &system; });
}

void World::Remove(VectorList<SomeWorldSystem*> const& systems) {
    for (auto& system : systems) {
        Remove(*system);
    }
}

Camera* World::MainCamera() const {
#ifdef PROFILE
    DevProfiler devProfiler("MainCamera", [](String value) { cout << value; });
#endif

    if (!mainCamera.expired()) {
        auto cameraResult = mainCamera.lock().get();

        // Make sure the cached reference isn't an orphan
        if (cameraResult->owner && cameraResult->owner->World()) {
            return cameraResult;
        }
    }

    VectorList<WorldNode*> graph;
    CollectBreadthFirstTree(root.get(), graph);

    for (auto& node : graph) {
        auto worldNode = node;
        auto camera = worldNode->TypeComponentPtr<Camera>();

        // Offscreen cameras don't qualify as the main camera
        GUARD_CONTINUE(camera && !camera->renderContext)

        mainCamera = camera;
        return camera.get();
    }

    return {};
}

void World::OnGo() {
    Base::OnGo();

#ifdef DEBUG
    std::cout << "PHASE JUMP IS DEBUG" << std::endl;
#endif
}

void World::Render() {
    // Render once before we limit rendering
    if (isRenderRateLimited) {
        renderLimiter.Fire();
    } else {
        RenderNow();
    }
}

void World::RenderNow() {
    didRender = true;

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

    isRemoveNodesLockedCount++;
    Defer defer([&]() { isRemoveNodesLockedCount -= 1; });

    VectorList<Camera*> optionalCameras;
    std::transform(
        updateList.begin(), updateList.end(), std::back_inserter(optionalCameras),
        [](WorldNode* node) {
            auto result = node->GetComponent<Camera>();
            return (result && result->IsEnabled()) ? result : nullptr;
        }
    );
    VectorList<Camera*> cameras;
    std::copy_if(
        optionalCameras.begin(), optionalCameras.end(), std::back_inserter(cameras),
        [](Camera* camera) { return camera != nullptr; }
    );

    RenderContextModel contextModel{ .renderContext = renderContext.get(),
                                     .root = root.get(),
                                     .nodes = updateList,
                                     .cameras = cameras };

    int drawCount = 0;

    // Prevent crash if system is removed during loop
    VectorList<SP<SomeWorldSystem>> iterSystems = systems;

    // FUTURE: re-evaluate using pointers (will crash if system is removed during loop)
    // FUTURE: Map<SomeWorldSystem*>(systems, [](auto& system) { return system.get(); });

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

    isRemoveNodesLockedCount++;
    Defer defer([&]() { isRemoveNodesLockedCount--; });

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

void World::Add(SP<WorldNode> const& node) {
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
        auto& prefab = prefabs.at(id);
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

ScreenPosition World::WorldToScreen(WorldPosition worldPos) const {
    auto camera = MainCamera();
    GUARDR(camera, {})

    auto result = camera->WorldToScreen(worldPos);
    return result;
}

#include "Matrix4x4.h"
#include "World.h"
#include "WorldNode.h"
#include <TSMatrix4D.h>

using namespace PJ;

WorldPosition PJ::ScreenToWorld(SomeWorldComponent const& component, ScreenPosition screenPos) {
    auto owner = component.Node();
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

    auto owner = component.Node();
    GUARDR(owner, result)

    auto world = owner->World();
    GUARDR(world, result)

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
    auto owner = component.Node();
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
