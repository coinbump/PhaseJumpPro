#include "BrushPaintTool.h"
#include "DragGestureHandler2D.h"
#include "PaintCanvas.h"
#include "SomeRenderContext.h"
#include "SpriteRenderer.h"
#include "Texture.h"
#include "Viewport.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

BrushPaintTool::BrushPaintTool() {
    onDragGestureUpdateFunc = [this](DragGestureHandler2D::Update update) {
        using UpdateType = DragGestureHandler2D::Update::Type;

        switch (update.type) {
        case UpdateType::Start:
            MakeBrushSprite();
            if (auto node = brushSpriteNode.lock()) {
                node->Enable(true);
                if (auto sprite = node->TypeComponent<SpriteRenderer>()) {
                    sprite->SetColor(canvas ? canvas->brushColor : Color::black);
                }
            }
            MoveBrushTo(update.position);
            break;
        case UpdateType::Update:
            MoveBrushTo(update.position);
            break;
        case UpdateType::End:
        case UpdateType::Cancel:
            if (auto node = brushSpriteNode.lock()) {
                node->Enable(false);
            }
            break;
        }
    };
}

void BrushPaintTool::MakeBrushSprite() {
    GUARD(canvas)
    GUARD(brushSpriteNode.expired())

    auto canvasVPNode = canvas->CanvasViewportNode();
    auto brushVPNode = canvas->BrushViewportNode();
    GUARD(canvasVPNode && brushVPNode)

    auto brushViewport = brushVPNode->TypeComponent<Viewport>();
    GUARD(brushViewport && brushViewport->renderContext)

    auto texture = brushViewport->renderContext->GetTexture();
    GUARD(texture)

    auto node = MAKE<WorldNode>(WorldNode::Config{ .name = "Brush Sprite" });
    canvasVPNode->Add(node);
    brushSpriteNode = node;

    auto& sprite = node->AddComponent<SpriteRenderer>(SpriteRenderer::Config{ .texture = texture });
    sprite.SetWorldSize(Vector3(30, 30, 0));
    node->Enable(false);
}

void BrushPaintTool::MoveBrushTo(Vector2 worldPos) {
    GUARD(canvas && canvas->Node())

    auto canvasOwnerPos = canvas->Node()->transform.WorldPosition();
    auto node = brushSpriteNode.lock();
    GUARD(node)

    // Sprite lives under canvasViewportNode (child at local origin of the canvas owner), so
    // its local position equals the drag offset from the canvas owner in outer-world coords.
    node->transform.SetLocalPosition(
        Vector3(worldPos.x - canvasOwnerPos.x, worldPos.y - canvasOwnerPos.y, 0)
    );
}
