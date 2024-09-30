#include "EventWorldSystem.h"
#include "DropFilesUIEvent.h"
#include "Input.h"
#include "Matrix4x4.h"
#include "SignalTypes.h"
#include "SomeCamera.h"
#include "SomeControllerUIEvent.h"
#include "SomePosition.h"
#include "SomeRaycaster2D.h"
#include "SomeUIEvent.h"
#include "World.h"
#include "WorldNode.h"
#include <iostream>
#include <TSMatrix4D.h>

using namespace std;
using namespace PJ;

EventWorldSystem::EventWorldSystem(String name) :
    Base(name),
    inputCollectFunc([](EventWorldSystem& system, List<WP<WorldNode>>& nodes) {
        GUARD(system.world && system.world->root)

        WorldNode::NodeList graph;
        CollectBreadthFirstTree(system.world->root, graph);

        std::transform(
            graph.begin(), graph.end(), std::back_inserter(nodes),
            [](SP<WorldNode> node) { return node; }
        );
    }) {}

VectorList<PJ::LocalHit> EventWorldSystem::TestLocalHit(ScreenPosition screenPosition) {
    GUARDR(world, {})

    // Get the camera
    SP<SomeCamera> camera = world->MainCamera();
    GUARDR(camera, {})

    // Get the raycaster
    auto raycaster = camera->owner->GetComponent<SomeRaycaster2D>();
    GUARDR(raycaster, {})

    auto worldPosition = camera->ScreenToWorld(screenPosition);
    // cout << "Log: Test: " << worldPosition.ToString() << std::endl;

    auto hits = raycaster->Raycast(worldPosition, vec2Zero);

    VectorList<PJ::LocalHit> result;
    for (auto& hit : hits) {
        auto worldHitPosition = camera->ScreenToWorld(screenPosition);
        auto worldModelMatrix = world->WorldModelMatrix(*hit.node);
        Terathon::Point3D point(worldHitPosition.x, worldHitPosition.y, worldHitPosition.z);
        auto localHitPosition = Terathon::InverseTransform(worldModelMatrix, point);

        LocalPosition localPosition(Vector3(localHitPosition.x, localHitPosition.y, 0));
        result.push_back(PJ::LocalHit(localPosition, hit.node));
    }
    return result;
}

void EventWorldSystem::OnPointerDown(PointerDownUIEvent const& pointerDownEvent) {}

void EventWorldSystem::OnPointerUp(PointerUpUIEvent const& pointerUpEvent) {}

void EventWorldSystem::ProcessUIEvents(List<SP<SomeUIEvent>> const& uiEvents) {
    SomeWorldSystem::ProcessUIEvents(uiEvents);

    GUARD(world && world->root)

    List<WP<WorldNode>> inputNodes;

    // Raycast and send pointer events directly to the node they hit
    for (auto& event : uiEvents) {
        if (inputMap) {
            auto firstAction = inputMap->FirstOutputFor(*event);
            if (firstAction && inputCollectFunc) {
                inputCollectFunc(*this, inputNodes);
                InputActionEvent event(firstAction.value());
                OnInputAction(event, inputNodes);
            }
        }

        auto controllerAxisEvent = DCAST<ControllerAxisUIEvent>(event);
        if (controllerAxisEvent) {
            Input::Update(*controllerAxisEvent);
            continue;
        }

        auto pointerDownEvent = DCAST<PointerDownUIEvent>(event);
        if (pointerDownEvent) {
            OnPointerDown(*pointerDownEvent);
            continue;
        }

        auto pointerUpEvent = DCAST<PointerUpUIEvent>(event);
        if (pointerUpEvent) {
            OnPointerUp(*pointerUpEvent);
            continue;
        }

        auto pointerMotionEvent = DCAST<PointerMoveUIEvent>(event);
        if (pointerMotionEvent) {
            OnMouseMotion(*pointerMotionEvent);
            continue;
        }

        auto keyDownEvent = DCAST<KeyDownUIEvent>(event);
        if (keyDownEvent && inputCollectFunc) {
            inputCollectFunc(*this, inputNodes);
            OnKeyDown(*keyDownEvent, inputNodes);
            continue;
        }

        auto dropFilesEvent = DCAST<DropFilesUIEvent>(event);
        if (dropFilesEvent) {
            inputCollectFunc(*this, inputNodes);
            OnDropFiles(*dropFilesEvent, inputNodes);
        }
    }
}

void EventWorldSystem::OnDropFiles(
    DropFilesUIEvent const& event, List<WP<WorldNode>> const& nodes
) {
    for (auto& node : nodes) {
        GUARD_CONTINUE(!node.expired())
        auto _node = node.lock();

        DispatchEvent(_node, SignalId::DropFiles, event);
    }
}

void EventWorldSystem::OnKeyDown(KeyDownUIEvent const& event, List<WP<WorldNode>> const& nodes) {
    for (auto& node : nodes) {
        GUARD_CONTINUE(!node.expired())
        auto _node = node.lock();

        DispatchEvent(_node, SignalId::KeyDown, event);
    }
}

void EventWorldSystem::OnInputAction(
    InputActionEvent const& event, List<WP<WorldNode>> const& nodes
) {
    for (auto& node : nodes) {
        GUARD_CONTINUE(!node.expired())
        auto _node = node.lock();

        DispatchEvent(_node, SignalId::InputAction, event);
    }
}

void EventWorldSystem::OnMouseMotion(PointerMoveUIEvent const& event) {}
