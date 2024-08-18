#include "ComponentTool.h"
#include "SomeRenderer.h"

using namespace std;
using namespace PJ;

void ComponentTool::AddComponentInternal(WorldNode& node, SP<SomeWorldComponent> component) {
    auto simplePolygonCollider2D = DCAST<PolygonCollider2D>(component);
    if (simplePolygonCollider2D && IsEmpty(simplePolygonCollider2D->poly)) {
        auto spriteRenderer = node.GetComponent<SomeRenderer>();

        // TODO: this won't work with atlas textures
        if (spriteRenderer && spriteRenderer->material &&
            !IsEmpty(spriteRenderer->material->Textures())) {
            float maxWidth = 0;
            float maxHeight = 0;
            for (auto& texture : spriteRenderer->material->Textures()) {
                maxWidth = max(maxWidth, (float)texture->size.x);
                maxHeight = max(maxHeight, (float)texture->size.y);
            }
            Vector2Int textureSize(maxWidth, maxHeight);

            simplePolygonCollider2D->poly.Add(
                Vector3(-textureSize.x / 2.0f, textureSize.y / 2.0f, 0)
            );
            simplePolygonCollider2D->poly.Add(Vector3(textureSize.x / 2.0f, textureSize.y / 2.0f, 0)
            );
            simplePolygonCollider2D->poly.Add(
                Vector3(textureSize.x / 2.0f, -textureSize.y / 2.0f, 0)
            );
            simplePolygonCollider2D->poly.Add(
                Vector3(-textureSize.x / 2.0f, -textureSize.y / 2.0f, 0)
            );
        }
        return;
    }
}
