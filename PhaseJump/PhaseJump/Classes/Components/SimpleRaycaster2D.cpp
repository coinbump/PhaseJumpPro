#include "SimpleRaycaster2D.h"
#include "Colliders2D.h"
#include "Log.h"
#include "Matrix4x4.h"
#include "SomeCamera.h"
#include "Utils.h"
#include "World.h"
#include <iostream>
#include <optional>

using namespace std;
using namespace PJ;

std::optional<RaycastHit2D> SimpleRaycaster2D::Raycast(Vector2 origin, Vector2 direction) {
    GUARDR(Node()->World(), std::optional<RaycastHit2D>())

    std::optional<RaycastHit2D> result;

    auto owner = this->owner;
    GUARDR(owner, result)
    auto ownerNode = owner;

    auto camera = ownerNode->GetComponent<SomeCamera>();
    if (nullptr == camera) {
        PJLog("ERROR. Raycaster requires camera.");
        return result;
    }

    // This is a brute force solution and not efficient
    // FUTURE: find more optimal raycast solutions as needed
    auto root = ownerNode->Root();
    GraphNodeTool<> graphNodeTool;
    auto graph = graphNodeTool.CollectBreadthFirstGraph(root);

    auto world = Node()->World();
    GUARDR(world, result)

    for (auto& node : graph) {
        auto worldNode = SCAST<WorldNode>(node);
        auto colliders = worldNode->GetComponents<PolygonCollider2D>();
        GUARD_CONTINUE(!IsEmpty(colliders))

        auto localToWorldMatrix = world->WorldModelMatrix(*worldNode);

        for (auto& collider : colliders) {
            auto polygon = collider->poly;

            for (int i = 0; i < polygon.size(); i++) {
                polygon[i] = localToWorldMatrix.MultiplyPoint(polygon[i]);
            }

            // cout << "Log: Test Poly: " << polygon.ToString() << std::endl;

            // TODO: we're not really raycasting here, we're just testing a single point
            if (polygon.TestHit(origin)) {
                // cout << "Log: Test Poly HIT\n";

                return std::make_optional<RaycastHit2D>(worldNode);
            }
        }
    }

    return result;
}
