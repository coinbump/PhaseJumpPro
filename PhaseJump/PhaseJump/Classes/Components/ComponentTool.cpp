#include "ComponentTool.h"
#include "SomeRenderer.h"

using namespace std;
using namespace PJ;

void ComponentTool::AddComponentInternal(WorldNode& node, SP<SomeWorldComponent> component) {
    auto simplePolygonCollider2D = DCAST<PolygonCollider2D>(component);
    if (simplePolygonCollider2D && IsEmpty(simplePolygonCollider2D->poly.value)) {
        auto renderer = node.GetComponent<SomeRenderer>();

        if (renderer) {
            auto worldSize = renderer->WorldSize();
            if (worldSize) {
                simplePolygonCollider2D->poly.Add(
                    Vector3((worldSize->x / 2.0f) * vecLeft, (worldSize->y / 2.0) * vecUp, 0)
                );
                simplePolygonCollider2D->poly.Add(
                    Vector3((worldSize->x / 2.0f) * vecRight, (worldSize->y / 2.0f) * vecUp, 0)
                );
                simplePolygonCollider2D->poly.Add(
                    Vector3((worldSize->x / 2.0f) * vecRight, (worldSize->y / 2.0f) * vecDown, 0)
                );
                simplePolygonCollider2D->poly.Add(
                    Vector3((worldSize->x / 2.0f) * vecLeft, (worldSize->y / 2.0f) * vecDown, 0)
                );
            }
        }

        return;
    }
}
