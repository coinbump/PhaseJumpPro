#include "SimpleRaycaster2D.h"
#include "SomeCamera.h"
#include "SimplePolygonCollider2D.h"
#include "Matrix4x4.h"
#include "Macros.h"
#include "World.h"
#include "Log.h"
#include <iostream>
#include <optional>

using namespace std;
using namespace PJ;

std::optional<RaycastHit2D> SimpleRaycaster2D::Raycast(Vector2 origin, Vector2 direction) {
    std::optional<RaycastHit2D> result;

    auto owner = this->owner.lock();
    if (nullptr == owner) { return result; }
    auto ownerNode = SCAST<WorldNode>(owner);

    auto world = World();

    auto camera = ownerNode->GetComponent<SomeCamera>();
    if (nullptr == camera) {
        PJLog("ERROR. Raycaster requires camera.");
        return result;
    }

    // This is a brute force solution and not efficient
    // FUTURE: find more optimal raycast solutions as needed
    auto root = ownerNode->Root();
    auto graph = root->CollectBreadthFirstGraph();

    for (auto node : graph) {
        auto worldNode = SCAST<WorldNode>(node);
        auto colliders = worldNode->GetComponents<SimplePolygonCollider2D>();
        if (colliders.size() == 0) { continue; }

        auto worldMatrix = world->WorldModelMatrix(*worldNode);

        for (auto collider : colliders) {
            auto polygon = collider->polygon;

            for (int i = 0; i < polygon.size(); i++) {
                polygon[i] = worldMatrix.MultiplyPoint(polygon[i]);
            }

            // cout << "Log: Test Poly: " << polygon.ToString() << "\n";

            if (polygon.TestHit(origin)) {
                // cout << "Log: Test Poly HIT\n";

                return std::make_optional<RaycastHit2D>(worldNode);
            }
        }
    }

    return result;
}
