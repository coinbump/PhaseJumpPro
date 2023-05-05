#include "EventSystem.h"
#include "SomeCamera.h"
#include "SomeRaycaster2D.h"
#include "SomeUIEvent.h"
#include "Matrix4x4.h"
#include <TSMatrix4D.h>
#include <iostream>

using namespace std;
using namespace PJ;

void EventSystem::ProcessUIEvents(VectorList<SP<SomeUIEvent>> const& uiEvents) {
    WorldSystem::ProcessUIEvents(uiEvents);

    // Raycast
    for (auto event : uiEvents) {
        auto pointerDownEvent = DCAST<PointerDownUIEvent<ScreenPosition>>(event);
        if (pointerDownEvent) {
            // Get the camera
            auto camera = Camera::main;
            if (nullptr == camera) { return; }

            // Get the raycaster
            auto raycaster = camera->GetComponent<SomeRaycaster2D>();
            if (nullptr == raycaster) { return; }

            auto screenPressPosition = pointerDownEvent->pressPosition;

            auto worldPosition = camera->ScreenToWorld(screenPressPosition.position);
            auto hit = raycaster->Raycast(worldPosition, Vector2::zero);
            if (!hit) { return; }

            auto worldPressPosition = camera->ScreenToWorld(screenPressPosition.position);
            auto worldModelMatrix = camera->WorldModelMatrix(*hit->node);
            Terathon::Point3D point(worldPressPosition.x, worldPressPosition.y, worldPressPosition.z);
            auto localPressPosition = Terathon::InverseTransform(worldModelMatrix, point);

            LocalPosition localPosition(Vector3(localPressPosition.x, localPressPosition.y, 0), hit->node);

            cout << "Hit! @ " << localPressPosition.x << ", " << localPressPosition.y << "\n";

            // Send mouse down event to object
            hit->node->OnPointerDownEvent(PointerDownUIEvent<LocalPosition>(localPosition, pointerDownEvent->button));
        }
    }
}
