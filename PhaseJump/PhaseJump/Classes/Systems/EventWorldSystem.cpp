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

std::optional<PJ::LocalHit> EventWorldSystem::TestLocalHit(ScreenPosition screenPosition) {
    GUARDR(world, std::nullopt)

    // Get the camera
    SP<SomeCamera> camera = world->MainCamera();
    GUARDR(camera, std::nullopt)

    // Get the raycaster
    auto raycaster = camera->owner->GetComponent<SomeRaycaster2D>();
    GUARDR(raycaster, std::nullopt)

    auto worldPosition = camera->ScreenToWorld(screenPosition);
    // cout << "Log: Test: " << worldPosition.ToString() << std::endl;

    auto hit = raycaster->Raycast(worldPosition, vec2Zero);
    if (!hit) {
        return std::nullopt;
    }

    auto worldHitPosition = camera->ScreenToWorld(screenPosition);
    auto worldModelMatrix = world->WorldModelMatrix(*hit->node);
    Terathon::Point3D point(worldHitPosition.x, worldHitPosition.y, worldHitPosition.z);
    auto localHitPosition = Terathon::InverseTransform(worldModelMatrix, point);

    LocalPosition localPosition(Vector3(localHitPosition.x, localHitPosition.y, 0));
    return make_optional(PJ::LocalHit(localPosition, hit->node));
}

void EventWorldSystem::OnPointerDown(PointerDownUIEvent const& pointerDownEvent) {
    // cout << "Log: OnPointerDown\n";

    auto screenPos = pointerDownEvent.screenPos;
    auto hit = TestLocalHit(pointerDownEvent.screenPos);
    GUARD(hit)
    GUARD(!hit->node.expired())

    // cout << "Log: HIT: OnPointerDown\n";

    // Send mouse down event to node components
    auto localPointerDownEvent = PointerDownUIEvent(screenPos, pointerDownEvent.button);
    auto hitNode = hit->node.lock();
    DispatchEvent(hitNode, SignalId::PointerDown, localPointerDownEvent);

    pointerDownNodesMap[pointerDownEvent.button].clear();
    Add(pointerDownNodesMap[pointerDownEvent.button], hitNode);
}

void EventWorldSystem::OnPointerUp(PointerUpUIEvent const& pointerUpEvent) {
    auto iterNodes = pointerDownNodesMap[pointerUpEvent.button];
    for (auto& node : iterNodes) {
        if (node.expired()) {
            continue;
        }

        auto _node = node.lock();

        // Send event to node components
        // TODO: Currently we aren't sending the local position
        auto localPointerUpEvent = PointerUpUIEvent(pointerUpEvent.button);
        DispatchEvent(_node, SignalId::PointerUp, localPointerUpEvent);
    }
}

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

void EventWorldSystem::OnMouseMotion(PointerMoveUIEvent const& event) {
    // This only supports a single object for enter/exit pointer
    // FUTURE: support multiple if needed
    auto hit = TestLocalHit(event.screenPos);
    if (!hit) {
        // Nothing was hit, send pointer exit events
        for (auto& node : pointerEnterNodes) {
            GUARD_CONTINUE(!node.expired())

            // cout << "Log: Pointer Exit: " <<
            // pointerMotionEvent->position.ToString() << std::endl;

            auto _node = node.lock();
            PointerExitUIEvent event;
            DispatchEvent(_node, SignalId::PointerExit, event);
        }
        pointerEnterNodes.clear();
    } else {
        // We hit something, send pointer exit events for the others and pointer enter for this
        auto hitNode = hit.value().node.lock();

        bool isHitInPointerEnterNodes = false;
        for (auto& node : pointerEnterNodes) {
            if (node.expired()) {
                continue;
            }

            auto _node = node.lock();
            if (_node == hitNode) {
                isHitInPointerEnterNodes = true;
                continue;
            }

            // cout << "Log: Pointer Exit: " <<
            // pointerMotionEvent->position.ToString() << std::endl;
            PointerExitUIEvent event;
            DispatchEvent(_node, SignalId::PointerExit, event);
        }
        pointerEnterNodes.clear();

        Add(pointerEnterNodes, hitNode);

        // The hit node already received a pointer enter event
        if (!isHitInPointerEnterNodes) {
            PointerEnterUIEvent event;
            DispatchEvent(hitNode, SignalId::PointerEnter, event);
        }
    }
}
