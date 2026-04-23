#include "PaintCanvas.h"
#include "ColorRenderer.h"
#include "DragGestureHandler2D.h"
#include "EllipseMeshBuilder.h"
#include "PaintTool.h"
#include "QuickBuilder.h"
#include "RendererModel.h"
#include "SomeRenderContext.h"
#include "SomeRenderEngine.h"
#include "SpriteRenderer.h"
#include "Texture.h"
#include "Updatables.h"
#include "Viewport.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

PaintCanvas::PaintCanvas(Config const& config) :
    pixelSize(config.pixelSize) {}

PaintCanvas::~PaintCanvas() {
    if (activeTool) {
        activeTool->canvas = nullptr;
    }
}

void PaintCanvas::Awake() {
    Base::Awake();

    QB(*this->owner).RectCollider(pixelSize);

    dragHandler = owner->AddComponentPtr<DragGestureHandler2D>();
    dragHandler->onDragGestureUpdateFunc = [this](auto update) {
        GUARD(activeTool && activeTool->onDragGestureUpdateFunc)
        activeTool->onDragGestureUpdateFunc(update);
    };

    // Brush viewport:
    // The viewport's texture becomes the brush stamp used by paint tools.
    auto _brushViewportNode = MAKE<WorldNode>(WorldNode::Config{ .name = "Brush Viewport" });
    owner->Add(_brushViewportNode);
    brushViewportNode = _brushViewportNode;

    auto brushViewport = _brushViewportNode->AddComponentPtr<Viewport>();
    // Transparent clear so the brush texture is just the brush
    brushViewport->clearColor = Color::clear;
    brushViewport->worldSize = Vector2(30, 30);

    auto brushCircleNode = MAKE<WorldNode>(WorldNode::Config{ .name = "Brush Circle" });
    _brushViewportNode->Add(brushCircleNode);

    auto& circleRenderer = brushCircleNode->AddComponent<ColorRenderer>(ColorRenderer::Config{
        .color = Color::white, .worldSize = Vector2(30, 30) });
    circleRenderer.EnableBlend(true);
    circleRenderer.core.model.SetBuildMeshFunc([](RendererModel const& model) {
        EllipseMeshBuilder builder(model.WorldSize());
        return builder.BuildMesh();
    });

    // Canvas viewport: starts cleared, then switches to no-clear so brush renders
    // accumulate across frames instead of being wiped out.
    auto _canvasViewportNode = MAKE<WorldNode>(WorldNode::Config{ .name = "Canvas Viewport" });
    owner->Add(_canvasViewportNode);
    canvasViewportNode = _canvasViewportNode;

    auto canvasViewport = _canvasViewportNode->AddComponentPtr<Viewport>();
    canvasViewport->worldSize = pixelSize;

    SP<Texture> canvasTexture;
    if (auto renderContext = DCAST<RenderContext>(canvasViewport->renderContext)) {
        canvasTexture = renderContext->GetTexture();

        renderContext->clearColor = Color::clear;
        renderContext->Bind();
        renderContext->Clear();
        renderContext->clearType = RenderClearType::None;
    }

    // Canvas render node: displays the canvas viewport texture on-screen
    auto _canvasRenderNode = MAKE<WorldNode>(WorldNode::Config{ .name = "Canvas Render" });
    owner->Add(_canvasRenderNode);
    canvasRenderNode = _canvasRenderNode;

    auto& canvasSprite = _canvasRenderNode->AddComponent<SpriteRenderer>(SpriteRenderer::Config{
        .texture = canvasTexture });
    canvasSprite.SetWorldSize(Vector3(pixelSize.x, pixelSize.y, 0));

    auto _toolsNode = MAKE<WorldNode>(WorldNode::Config{ .name = "Tools" });
    owner->Add(_toolsNode);
    toolsNode = _toolsNode;
}

void PaintCanvas::SwapCanvas(SP<Texture> texture) {
    GUARD(texture)

    auto vpNode = canvasViewportNode.lock();
    GUARD(vpNode)
    auto viewport = vpNode->TypeComponent<Viewport>();
    GUARD(viewport)

    viewport->Swap(texture);

    auto size = texture->Size();
    pixelSize = Vector2(size.x, size.y);

    auto renderNode = canvasRenderNode.lock();
    GUARD(renderNode)
    auto sprite = renderNode->TypeComponent<SpriteRenderer>();
    GUARD(sprite)

    sprite->SetWorldSize(Vector3(pixelSize.x, pixelSize.y, 0));

    auto material = sprite->core.model.material;
    if (viewport->renderContext && material) {
        material->SetTexture(viewport->renderContext->GetTexture());
        sprite->core.model.SetMeshNeedsBuild();
    }
}

void PaintCanvas::SetActiveTool(SP<PaintTool> value) {
    if (activeTool) {
        activeTool->canvas = nullptr;
    }

    activeTool = value;

    if (activeTool) {
        activeTool->canvas = this;
    }
}
