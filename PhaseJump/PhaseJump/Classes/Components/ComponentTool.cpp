#include "ComponentTool.h"

using namespace std;
using namespace PJ;

void ComponentTool::AddComponentInternal(WorldNode& node, SP<SomeWorldComponent> component) {
    auto simplePolygonCollider2D = dynamic_pointer_cast<SimplePolygonCollider2D>(component);
    if (simplePolygonCollider2D && simplePolygonCollider2D->polygon.IsEmpty()) {
        auto spriteRenderer = node.GetComponent<SpriteRenderer>();
        if (spriteRenderer && spriteRenderer->material && !spriteRenderer->material->textures.IsEmpty()) {
            auto textureSize = spriteRenderer->material->textures[0]->size;

            simplePolygonCollider2D->polygon.Add(Vector3(-textureSize.x / 2.0f, textureSize.y / 2.0f, 0));
            simplePolygonCollider2D->polygon.Add(Vector3(textureSize.x / 2.0f, textureSize.y / 2.0f, 0));
            simplePolygonCollider2D->polygon.Add(Vector3(-textureSize.x / 2.0f, -textureSize.y / 2.0f, 0));
            simplePolygonCollider2D->polygon.Add(Vector3(textureSize.x / 2.0f, -textureSize.y / 2.0f, 0));
        }
        return;
    }
}
