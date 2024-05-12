#include "EventSystem.h"
#include "SomeCamera.h"
#include "SomeRaycaster2D.h"
#include "SomeUIEvent.h"
#include "Matrix4x4.h"
#include "WorldNode.h"
#include "SomePosition.h"
#include "World.h"
#include <TSMatrix4D.h>
#include <iostream>

using namespace std;
using namespace PJ;

std::optional<LocalPosition> EventSystem::LocalHitPosition(ScreenPosition screenPosition) {
    // Get the camera
    auto camera = Camera::main;
    if (nullptr == camera) { return std::nullopt; }

    // Get the raycaster
    auto raycaster = camera->GetComponent<SomeRaycaster2D>();
    if (nullptr == raycaster) { return std::nullopt; }

    auto worldPosition = camera->ScreenToWorld(screenPosition);
    // cout << "Log: Test: " << worldPosition.ToString() << "\n";

    auto hit = raycaster->Raycast(worldPosition, Vector2::zero);
    if (!hit) { return std::nullopt; }

    auto worldHitPosition = camera->ScreenToWorld(screenPosition);
    auto worldModelMatrix = World()->WorldModelMatrix(*hit->node);
    Terathon::Point3D point(worldHitPosition.x, worldHitPosition.y, worldHitPosition.z);
    auto localHitPosition = Terathon::InverseTransform(worldModelMatrix, point);

    LocalPosition localPosition(Vector3(localHitPosition.x, localHitPosition.y, 0), hit->node);
    return make_optional(localPosition);
}

bool EventSystem::ProcessPointerDownEvent(SP<PointerDownUIEvent<ScreenPosition>> pointerDownEvent) {
    if (nullptr == pointerDownEvent) { return false; }

    auto result = true;

    // cout << "Log: ProcessPointerDownEvent\n";

    auto hitPosition = LocalHitPosition(pointerDownEvent->pressPosition);
    if (!hitPosition) { return result; }

    // cout << "Log: HIT: ProcessPointerDownEvent\n";

    // Send mouse down event to node components
    auto localPointerDownEvent = PointerDownUIEvent<LocalPosition>(hitPosition.value(), pointerDownEvent->button);
    auto hitNode = hitPosition->reference.lock();

    DispatchEvent(hitNode, [&](SP<SomePointerEventsResponder> pointerEventsResponder) {
        pointerEventsResponder->OnPointerDownEvent(localPointerDownEvent);
    });

    pointerDownNodesMap[pointerDownEvent->button].clear();
    pointerDownNodesMap[pointerDownEvent->button].Add(hitNode);

    return result;
}

bool EventSystem::ProcessPointerUpEvent(SP<PointerUpUIEvent> pointerUpEvent) {
    if (nullptr == pointerUpEvent) { return false; }

    auto iterNodes = pointerDownNodesMap[pointerUpEvent->button];
    for (auto node : iterNodes) {
        if (node.expired()) { continue; }

        auto _node = node.lock();

        DispatchEvent(_node, [&](SP<SomePointerEventsResponder> pointerEventsResponder) {
            pointerEventsResponder->OnPointerUpEvent(*pointerUpEvent);
        });
    }

    return true;
}

void EventSystem::ProcessUIEvents(VectorList<SP<SomeUIEvent>> const& uiEvents) {
    WorldSystem::ProcessUIEvents(uiEvents);

    // Raycast
    for (auto event : uiEvents) {
        if (ProcessPointerDownEvent(DCAST<PointerDownUIEvent<ScreenPosition>>(event))) {
            return;
        }

        if (ProcessPointerUpEvent(DCAST<PointerUpUIEvent>(event))) {
            return;
        }

        // This only supports a single object for enter/exit pointer
        // FUTURE: support multiple if needed
        auto mouseMotionEvent = DCAST<MouseMotionUIEvent<ScreenPosition>>(event);
        if (mouseMotionEvent) {
            auto hitPosition = LocalHitPosition(mouseMotionEvent->position);
            if (!hitPosition) {
                for (auto node : pointerEnterNodes) {
                    if (node.expired()) { continue; }

                    // cout << "Log: Pointer Exit: " << mouseMotionEvent->position.ToString() << "\n";

                    auto _node = node.lock();
                    DispatchEvent(_node, [&](SP<SomePointerEventsResponder> pointerEventsResponder) {
                        pointerEventsResponder->OnPointerExitEvent(PointerExitUIEvent());
                    });
                }
                pointerEnterNodes.clear();
                return;
            } else {
                auto node = hitPosition.value().reference.lock();
                if (pointerEnterNodes.ContainsWhere([&](auto value) { return value.lock() == node; })) {
                    return;
                }

                // cout << "Log: Pointer Enter: " << hitPosition.value().ToString() << "\n";

                pointerEnterNodes.Add(node);

                DispatchEvent(node, [&](SP<SomePointerEventsResponder> pointerEventsResponder) {
                    pointerEventsResponder->OnPointerEnterEvent(PointerEnterUIEvent());
                });
            }

            return;
        }
    }
}

void EventSystem::DispatchEvent(SPC<WorldNode> node, std::function<void(SP<SomePointerEventsResponder>)> command) {
    auto iterComponents = node->Components();
    for (auto component : iterComponents) {
        auto pointerEventsResponder = DCAST<SomePointerEventsResponder>(component);
        if (nullptr == pointerEventsResponder) { continue; }

        command(pointerEventsResponder);
    }
}
