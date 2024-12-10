#include "World.h"
#include "DesignSystem.h"
#include "DevProfiler.h"
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

void World::Remove(SomeWorldSystem& system) {
    GUARD(system.World() == this)

    system.SetWorld(nullptr);
    RemoveFirstIf(systems, [&](auto& i) { return i.get() == &system; });
}

void World::Remove(VectorList<SP<SomeWorldSystem>> systems) {
    for (auto& system : systems) {
        Remove(*system);
    }
}

SP<SomeCamera> World::MainCamera() {
#ifdef PROFILE
    DevProfiler devProfiler("MainCamera", [](String value) { cout << value; });
#endif

    GUARDR(!mainCamera, mainCamera)

    VectorList<WorldNode*> graph;
    CollectBreadthFirstTree(root.get(), graph);

    for (auto& node : graph) {
        auto worldNode = node;
        auto camera = worldNode->GetComponent<SomeCamera>();

        // Offscreen cameras don't qualify as the main camera
        GUARD_CONTINUE(camera && !camera->renderContext)

        mainCamera = camera ? SCAST<SomeCamera>(camera->shared_from_this()) : nullptr;
        return mainCamera;
    }

    return nullptr;
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

    RenderContextModel contextModel(renderContext.get(), root.get(), nodes, cameras);

    int drawCount = 0;

    // Prevent iteration-mutation crash
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

void UpdateWorld(WorldNode& node, TimeSlice time, WorldNode::NodeList& updateList) {
    if (node.IsDestroyed()) {
        node.OnDestroy();

        // TODO: send OnDestroy to children
        GUARD(node.Parent())
        node.Parent()->Remove(node);
    } else {
        GUARD(node.IsEnabled())
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
    SP<SomeCamera> camera = world->MainCamera();
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
    SP<SomeCamera> camera = world->MainCamera();
    GUARDR(camera, result)

    auto worldPosition = camera->ScreenToWorld(screenPos);

    auto worldModelMatrix = world->WorldModelMatrix(*owner);
    Terathon::Point3D point(worldPosition.x, worldPosition.y, worldPosition.z);
    auto localPosition = Terathon::InverseTransform(worldModelMatrix, point);

    result = LocalPosition(localPosition.x, localPosition.y, 0);

    return result;
}

SP<SomeTexture> World::FindTexture(String id) {
    try {
        auto& typeMap = loadedResources->map.at("texture");
        auto result = typeMap.at(id);
        return DCAST<SomeTexture>(result.resource);
    } catch (...) {
        return nullptr;
    }
}

UnorderedSet<SP<SomeTexture>> World::FindTextures(UnorderedSet<String> const& textureIds) {
    VectorList<SP<SomeTexture>> result;

    std::transform(
        textureIds.begin(), textureIds.end(), std::inserter(result, result.begin()),
        [&](auto& id) { return FindTexture(id); }
    );
    Compact(result);

    UnorderedSet<SP<SomeTexture>> _result(result.begin(), result.end());
    return _result;
}

VectorList<SP<SomeTexture>> World::FindTextureList(VectorList<String> const& textureIds) {
    VectorList<SP<SomeTexture>> result;

    std::transform(
        textureIds.begin(), textureIds.end(), std::inserter(result, result.begin()),
        [&](auto& id) { return FindTexture(id); }
    );
    Compact(result);

    return result;
}

SP<Font> World::FindFontWithSize(float size) {
    FontSpec spec{ .size = size };
    return FindFont(spec);
}

SP<Font> World::FindFontWithResourceId(String id) {
    FontSpec spec{ .resourceId = id };
    return FindFont(spec);
}

SP<Font> World::FindFont(FontSpec spec) {
    try {
        auto& typeMap = loadedResources->map.at("font");
        if (spec.resourceId.size() > 0) {
            auto result = typeMap.at(spec.resourceId);
            return DCAST<Font>(result.resource);
        }

        VectorList<SP<Font>> candidates;
        for (auto const& item : typeMap) {
            auto font = DCAST<Font>(item.second.resource);
            GUARD_CONTINUE(font)
            candidates.push_back(font);
        }

        GUARDR(!IsEmpty(candidates), nullptr)

        // TODO: need unit test
        std::sort(
            candidates.begin(), candidates.end(),
            [&](SP<Font> const& lhs, SP<Font> const& rhs) {
                auto Weight = [](FontSpec spec, SP<Font> const& font) {
                    int result = 0;
                    if (!IsEmpty(spec.fontName)) {
                        if (font->name == spec.fontName) {
                            result++;
                        }
                    }
                    if (spec.size > 0) {
                        if (font->size == spec.size) {
                            result += 2;
                        }
                    }

                    return result;
                };

                int leftWeight = Weight(spec, lhs);
                int rightWeight = Weight(spec, rhs);

                return leftWeight > rightWeight;
            }
        );

        return candidates[0];
    } catch (...) {
        return nullptr;
    }
}

SP<SomeShaderProgram> World::FindShader(String id) {
    try {
        return SomeShaderProgram::registry.at(id);
    } catch (...) {
        return nullptr;
    }
}
