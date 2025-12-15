#include "EventWorldSystem.h"
#include "DropFilesUIEvent.h"
#include "Input.h"
#include "Matrix4x4.h"
#include "SignalTypes.h"
#include "SomeCamera.h"
#include "SomeControllerUIEvent.h"
#include "SomeKeyUIEvent.h"
#include "SomePosition.h"
#include "SomeRaycaster2D.h"
#include "World.h"
#include "WorldNode.h"
#include <iostream>
#include <TSMatrix4D.h>

using namespace std;
using namespace PJ;

EventWorldSystem::EventWorldSystem(String name) :
    Base(name),
    collectEventNodesFunc([](EventWorldSystem& system, EventNodeList& nodes) {
        nodes.clear();
        GUARD(system.world && system.world->Root())

        CollectBreadthFirstTree(system.world->Root(), nodes);
        nodes = Filter(nodes, [](auto& node) { return node->isListener; });
    }) {}

VectorList<PJ::LocalHit> EventWorldSystem::TestScreenHit(ScreenPosition screenPosition) {
    GUARDR(world, {})

    auto camera = world->MainCamera();
    GUARDR_LOG(camera, {}, "ERROR: Camera required for TestScreenHit")

    auto raycaster = camera->owner->GetComponent<SomeRaycaster2D>();
    GUARDR_LOG(raycaster, {}, "ERROR: Raycaster required for TestScreenHit")

    auto worldPosition = camera->ScreenToWorld(screenPosition);
    // cout << "Log: Test: " << worldPosition.ToString() << std::endl;

    auto hits = raycaster->Raycast(worldPosition, {});

    VectorList<PJ::LocalHit> result;
    for (auto& hit : hits) {
        auto worldHitPosition = camera->ScreenToWorld(screenPosition);
        auto worldModelMatrix = world->WorldModelMatrix(hit.node);
        Terathon::Point3D point(worldHitPosition.x, worldHitPosition.y, worldHitPosition.z);
        auto localHitPosition = Terathon::InverseTransform(worldModelMatrix, point);

        LocalPosition localPosition(Vector3(localHitPosition.x, localHitPosition.y, 0));
        result.push_back(PJ::LocalHit(localPosition, hit.node));
    }
    return result;
}

void EventWorldSystem::OnPointerDown(PointerDownUIEvent const& pointerDownEvent) {}

void EventWorldSystem::OnPointerUp(PointerUpUIEvent const& pointerUpEvent) {}

void EventWorldSystem::ProcessUIEvents(UIEventList const& uiEvents) {
    SomeWorldSystem::ProcessUIEvents(uiEvents);

    GUARD(world && world->Root())

    EventNodeList eventNodes;
    if (collectEventNodesFunc) {
        collectEventNodesFunc(*this, eventNodes);
    }

    // Raycast and send pointer events directly to the node they hit
    for (auto& event : uiEvents) {
        if (inputMap) {
            auto firstAction = inputMap->map.FirstOutputFor(*event);
            if (firstAction) {
                OnInputAction(*event, firstAction.value(), eventNodes);
            }
        }

        auto controllerAxisEvent = dynamic_cast<ControllerAxisUIEvent*>(event.get());
        if (controllerAxisEvent) {
            Input::Update(*controllerAxisEvent);
            continue;
        }

        auto pointerDownEvent = dynamic_cast<PointerDownUIEvent*>(event.get());
        if (pointerDownEvent) {
            OnPointerDown(*pointerDownEvent);
            continue;
        }

        auto pointerUpEvent = dynamic_cast<PointerUpUIEvent*>(event.get());
        if (pointerUpEvent) {
            OnPointerUp(*pointerUpEvent);
            continue;
        }

        auto pointerMotionEvent = dynamic_cast<PointerMoveUIEvent*>(event.get());
        if (pointerMotionEvent) {
            OnPointerMove(*pointerMotionEvent);
            continue;
        }

        auto keyDownEvent = dynamic_cast<KeyDownUIEvent*>(event.get());
        if (keyDownEvent && collectEventNodesFunc) {
            OnKeyDown(*keyDownEvent, eventNodes);
            continue;
        }

        auto dropFilesEvent = dynamic_cast<DropFilesUIEvent*>(event.get());
        if (dropFilesEvent) {
            OnDropFiles(*dropFilesEvent, eventNodes);
            continue;
        }

        auto windowResizeEvent = dynamic_cast<WindowResizeUIEvent*>(event.get());
        if (windowResizeEvent) {
            DispatchEvent(SignalId::WindowResize, *windowResizeEvent, eventNodes);
            continue;
        }
    }
}

void EventWorldSystem::DispatchEvent(
    String signalId, SomeSignal const& signal, EventNodeList const& nodes
) {
    GUARD(world)

    // TODO: dispatching every event to every node is inefficient and needs to be fixed
    for (auto& node : nodes) {
        DispatchEvent(*node, signalId, signal);
    }

    for (auto& system : world->Systems()) {
        // FUTURE: GUARD(system->IsEnabled())
        try {
            system->Signal(signalId, signal);
        } catch (...) {}
    }
}

void EventWorldSystem::OnDropFiles(DropFilesUIEvent const& event, EventNodeList const& nodes) {
    DispatchEvent(SignalId::DropFiles, event, nodes);
}

void EventWorldSystem::OnKeyDown(KeyDownUIEvent const& event, EventNodeList const& nodes) {
    DispatchEvent(SignalId::KeyDown, event, nodes);
}

void EventWorldSystem::OnInputAction(
    SomeSignal const& event, String action, EventNodeList const& nodes
) {
    // Inputs are dispatched by id. Example: "input.button.north"
    auto actionSignalId = "input." + action;
    DispatchEvent(actionSignalId, event, nodes);
}

void EventWorldSystem::OnPointerMove(PointerMoveUIEvent const& event) {}

void EventWorldSystem::DispatchEvent(WorldNode& node, String signalId, SomeSignal const& signal) {
    GUARD(node.IsEnabled());

    node.Signal(signalId, signal);
}
