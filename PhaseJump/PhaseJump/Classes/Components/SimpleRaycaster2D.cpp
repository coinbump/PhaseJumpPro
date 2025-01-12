#include "SimpleRaycaster2D.h"
#include "Colliders2D.h"
#include "Dev.h"
#include "Matrix4x4.h"
#include "SomeCamera.h"
#include "Utils.h"
#include "World.h"
#include <iostream>
#include <optional>

using namespace std;
using namespace PJ;

VectorList<RaycastHit2D> SimpleRaycaster2D::Raycast(Vector2 worldPosition, Vector2 direction) {
    // NOTE: true collision support will come later. This is just a brute force approach to get the
    // UI working
    VectorList<RaycastHit2D> result;
    GUARDR(owner && owner->World(), result)

    auto world = owner->World();

    auto camera = owner->GetComponent<SomeCamera>();
    if (nullptr == camera) {
        PJ::Log("ERROR. Raycaster requires camera.");
        return result;
    }

    // FUTURE: find more optimal raycast solutions as needed
    auto root = world->Root();

    VectorList<WorldNode*> graph;
    CollectBreadthFirstTree(root, graph);

    for (auto& node : graph) {
        auto colliders = node->GetComponents<SomeCollider2D>();
        GUARD_CONTINUE(!IsEmpty(colliders))

        auto localPos = node->WorldToLocal(worldPosition);

        for (auto& collider : colliders) {
            if (collider->TestHit(localPos)) {
                result.push_back(*node);
            }
        }
    }

    return result;
}
