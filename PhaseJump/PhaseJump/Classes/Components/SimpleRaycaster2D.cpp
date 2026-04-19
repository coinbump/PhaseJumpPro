#include "SimpleRaycaster2D.h"
#include "Camera.h"
#include "Colliders2D.h"
#include "Dev.h"
#include "Matrix4x4.h"
#include "Utils.h"
#include "Viewport.h"
#include "World.h"
#include <iostream>
#include <optional>

using namespace std;
using namespace PJ;

namespace {
    /// Returns false if any strict ancestor of `node` has a Viewport component AND
    /// `worldPosition` lies outside that viewport's local-space bounds. The viewport acts
    /// as a clipping rect for hit-tests against its descendants — clicks outside the
    /// viewport's visible area should not reach colliders inside it.
    bool IsWithinViewportClip(WorldNode const* node, Vector2 worldPosition) {
        GUARDR(node, true)

        for (auto cursor = node->Parent(); cursor; cursor = cursor->Parent()) {
            auto viewport = cursor->TypeComponent<Viewport>();
            GUARD_CONTINUE(viewport)

            auto localPos = cursor->WorldToLocal(worldPosition);
            auto size = viewport->worldSize.Value();
            float halfWidth = size.x * 0.5f;
            float halfHeight = size.y * 0.5f;

            if (localPos.x < -halfWidth || localPos.x > halfWidth || localPos.y < -halfHeight ||
                localPos.y > halfHeight) {
                return false;
            }
        }
        return true;
    }
} // namespace

VectorList<RaycastHit2D> SimpleRaycaster2D::Raycast(Vector2 worldPosition, Vector2 direction) {
    // NOTE: true collision support will come later. This is just a brute force approach to get the
    // UI working
    VectorList<RaycastHit2D> result;
    GUARDR(owner && owner->World(), result)

    auto world = owner->World();

    auto camera = owner->GetComponent<Camera>();
    if (nullptr == camera) {
        PJ::Log("ERROR. Raycaster requires camera.");
        return result;
    }

    // FUTURE: find more optimal raycast solutions as needed
    auto root = world->Root();

    VectorList<WorldNode*> graph;
    CollectBreadthFirstTree(root, graph);

    for (auto& node : graph) {
        GUARD_CONTINUE(node->IsEnabled())

        auto colliders = node->GetComponents<Collider2D>();
        GUARD_CONTINUE(!IsEmpty(colliders))

        // Viewport clipping: if any ancestor viewport excludes `worldPosition` from its
        // visible bounds, skip every collider on this node — the collider lives inside
        // the viewport's clipped subtree and isn't hittable from outside.
        GUARD_CONTINUE(IsWithinViewportClip(node, worldPosition))

        auto localPos = node->WorldToLocal(worldPosition);

        for (auto& collider : colliders) {
            GUARD_CONTINUE(collider->IsEnabled())

            if (collider->TestHit(localPos)) {
                result.push_back(*node);
            }
        }
    }

    return result;
}
