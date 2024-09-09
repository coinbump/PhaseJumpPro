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
    std::optional<RaycastHit2D> result;
    GUARDR(owner && owner->World(), result)

    auto owner = this->owner;
    GUARDR(owner, result)
    auto ownerNode = owner;

    auto camera = ownerNode->GetComponent<SomeCamera>();
    if (nullptr == camera) {
        PJLog("ERROR. Raycaster requires camera.");
        return result;
    }

    auto world = owner->World();
    GUARDR(world, result)

    // This is a brute force solution and not efficient
    // FUTURE: find more optimal raycast solutions as needed
    auto root = world->root;

    WorldNode::NodeList graph;
    CollectBreadthFirstTree(root, graph);

    for (auto& node : graph) {
        auto worldNode = SCAST<WorldNode>(node);
        auto colliders = worldNode->GetComponents<SomeCollider2D>();
        GUARD_CONTINUE(!IsEmpty(colliders))

        auto localToWorldMatrix = world->WorldModelMatrix(*worldNode);
        auto nodeWorldPosition = node->transform.WorldPosition();

        for (auto& collider : colliders) {
            auto polyCollider = As<PolygonCollider2D>(collider.get());
            if (polyCollider) {
                auto polygon = polyCollider->poly;

                for (int i = 0; i < polygon.Count(); i++) {
                    polygon[i] = localToWorldMatrix.MultiplyPoint(polygon[i]);
                }

                // cout << "Log: Test Poly: " << polygon.ToString() << std::endl;

                // TODO: we're not really raycasting here, we're just testing a single point
                if (polygon.TestHit(origin)) {
                    // cout << "Log: Test Poly HIT\n";

                    return std::make_optional<RaycastHit2D>(worldNode);
                }
                continue;
            }

            // TODO: add unit tests
            auto circleCollider = As<CircleCollider2D>(collider.get());
            if (circleCollider) {
                if (circleCollider->TestHit(origin - nodeWorldPosition)) {
                    return std::make_optional<RaycastHit2D>(worldNode);
                }
            }
        }
    }

    return result;
}
