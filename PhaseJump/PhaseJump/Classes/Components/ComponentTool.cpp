#include "ComponentTool.h"
#include "SomeRenderer.h"

using namespace std;
using namespace PJ;

void ComponentTool::AddComponentInternal(WorldNode& node, SP<SomeWorldComponent> component) {
    auto simplePolygonCollider2D = DCAST<PolygonCollider2D>(component);
    if (simplePolygonCollider2D && IsEmpty(simplePolygonCollider2D->poly)) {
        auto renderer = node.GetComponent<SomeRenderer>();

        if (renderer) {
            auto worldSize = renderer->WorldSize();
            if (worldSize) {
                Add(simplePolygonCollider2D->poly, Vector3(
                                                       (worldSize->x / 2.0f) * Vector2::left.x,
                                                       (worldSize->y / 2.0) * Vector2::up.y, 0
                                                   ));
                Add(simplePolygonCollider2D->poly, Vector3(
                                                       (worldSize->x / 2.0f) * Vector2::right.x,
                                                       (worldSize->y / 2.0f) * Vector2::up.y, 0
                                                   ));
                Add(simplePolygonCollider2D->poly, Vector3(
                                                       (worldSize->x / 2.0f) * Vector2::right.x,
                                                       (worldSize->y / 2.0f) * Vector2::down.y, 0
                                                   ));
                Add(simplePolygonCollider2D->poly, Vector3(
                                                       (worldSize->x / 2.0f) * Vector2::left.x,
                                                       (worldSize->y / 2.0f) * Vector2::down.y, 0
                                                   ));
            }
        }
        return;
    }
}
