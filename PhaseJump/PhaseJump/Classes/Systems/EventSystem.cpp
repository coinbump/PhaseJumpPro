#include "EventSystem.h"
#include "Input.h"
#include "Matrix4x4.h"
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

EventSystem::EventSystem() :
    inputCollectFunc([](EventSystem& system, List<WP<WorldNode>>& nodes) {
        GUARD(system.world && system.world->root)

        GraphNodeTool<> tool;
        auto collectedNodes = tool.CollectBreadthFirstGraph(system.world->root);
        std::transform(
            collectedNodes.begin(), collectedNodes.end(), std::back_inserter(nodes),
            [](SP<SomeGraphNode<>> node) { return SCAST<WorldNode>(node); }
        );
    }) {}

std::optional<PJ::LocalHit> EventSystem::TestLocalHit(ScreenPosition screenPosition) {
    GUARDR(world, std::nullopt)

    // Get the camera
    SP<SomeCamera> camera = world->MainCamera();
    GUARDR(camera, std::nullopt)

    // Get the raycaster
    auto raycaster = camera->GetComponent<SomeRaycaster2D>();
    GUARDR(raycaster, std::nullopt)

    auto worldPosition = camera->ScreenToWorld(screenPosition);
    // cout << "Log: Test: " << worldPosition.ToString() << std::endl;

    auto hit = raycaster->Raycast(worldPosition, Vector2::zero);
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

void EventSystem::OnPointerDown(PointerDownUIEvent const& pointerDownEvent) {
    // cout << "Log: OnPointerDown\n";

    auto screenPos = pointerDownEvent.screenPos;
    auto hit = TestLocalHit(pointerDownEvent.screenPos);
    GUARD(hit)
    GUARD(!hit->node.expired())

    // cout << "Log: HIT: OnPointerDown\n";

    // Send mouse down event to node components
    auto localPointerDownEvent = PointerDownUIEvent(screenPos, pointerDownEvent.button);
    auto hitNode = hit->node.lock();

    DispatchEvent<SomePointerUIEventsResponder>(
        hitNode, [&](SomePointerUIEventsResponder& responder
                 ) { responder.OnPointerDown(localPointerDownEvent); }
    );

    pointerDownNodesMap[pointerDownEvent.button].clear();
    pointerDownNodesMap[pointerDownEvent.button].Add(hitNode);
}

void EventSystem::OnPointerUp(PointerUpUIEvent const& pointerUpEvent) {
    auto iterNodes = pointerDownNodesMap[pointerUpEvent.button];
    for (auto& node : iterNodes) {
        if (node.expired()) {
            continue;
        }

        auto _node = node.lock();

        // Send event to node components
        // TODO: Currently we aren't sending the local position
        auto localPointerUpEvent = PointerUpUIEvent(pointerUpEvent.button);

        DispatchEvent<SomePointerUIEventsResponder>(
            _node, [&](SomePointerUIEventsResponder& responder
                   ) { responder.OnPointerUp(localPointerUpEvent); }
        );
    }
}

void EventSystem::ProcessUIEvents(List<SP<SomeUIEvent>> const& uiEvents) {
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
    }
}

void EventSystem::OnKeyDown(KeyDownUIEvent const& event, List<WP<WorldNode>> const& nodes) {
    for (auto& node : nodes) {
        GUARD_CONTINUE(!node.expired())
        auto _node = node.lock();

        DispatchEvent<SomeKeyUIEventsResponder>(_node, [&](SomeKeyUIEventsResponder& responder) {
            responder.OnKeyDown(event);
        });
    }
}

void EventSystem::OnInputAction(InputActionEvent const& event, List<WP<WorldNode>> const& nodes) {
    for (auto& node : nodes) {
        GUARD_CONTINUE(!node.expired())
        auto _node = node.lock();

        DispatchEvent<SomeInputActionEventResponder>(
            _node, [&](SomeInputActionEventResponder& responder) { responder.OnInputAction(event); }
        );
    }
}

void EventSystem::OnMouseMotion(PointerMoveUIEvent const& event) {
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
            DispatchEvent<SomePointerUIEventsResponder>(
                _node, [&](SomePointerUIEventsResponder& responder
                       ) { responder.OnPointerExit(PointerExitUIEvent()); }
            );
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

            DispatchEvent<SomePointerUIEventsResponder>(
                _node, [&](SomePointerUIEventsResponder& responder
                       ) { responder.OnPointerExit(PointerExitUIEvent()); }
            );
        }
        pointerEnterNodes.clear();

        pointerEnterNodes.Add(hitNode);

        // The hit node already received a pointer enter event
        if (!isHitInPointerEnterNodes) {
            DispatchEvent<SomePointerUIEventsResponder>(
                hitNode, [&](SomePointerUIEventsResponder& responder
                         ) { responder.OnPointerEnter(PointerEnterUIEvent()); }
            );
        }
    }
}
