#include "SelectObjectsCanvasTool.h"
#include "ColorRenderer.h"
#include "DragGestureHandler2D.h"
#include "ObjectCanvas.h"
#include "PolyFrameMeshBuilder.h"
#include "Polygon.h"
#include "RendererModel.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

SelectObjectsCanvasTool::SelectObjectsCanvasTool() {
    onDragGestureUpdateFunc = [this](DragGestureHandler2D::Update update) {
        switch (update.type) {
        case DragGestureHandler2D::Update::Type::Start:
            {
                GUARD(canvas)
                auto toolsNode = canvas->ToolsNode();
                GUARD(toolsNode)

                dragStartPosition = update.startPosition;
                dragCurrentPosition = update.position;

                auto renderer = toolsNode->AddComponentPtr<ColorRenderer>(ColorRenderer::Config{
                    .color = Color::black });
                renderer->EnableBlend(true);

                renderer->core.SetBuildMeshFunc([this](RendererModel const& model) {
                    auto const& start = dragStartPosition;
                    auto const& current = dragCurrentPosition;

                    Polygon poly;
                    poly.Add({ start.x, start.y });
                    poly.Add({ current.x, start.y });
                    poly.Add({ current.x, current.y });
                    poly.Add({ start.x, current.y });

                    PolyFrameMeshBuilder builder(PolyFrameMeshBuilder::Config{ .poly = poly,
                                                                               .strokeWidth = 3 });
                    return builder.BuildMesh();
                });

                marqueeRenderer = renderer;
                break;
            }
        case DragGestureHandler2D::Update::Type::Update:
            {
                dragCurrentPosition = update.position;
                auto renderer = marqueeRenderer.lock();
                GUARD(renderer)
                renderer->core.model.SetMeshNeedsBuild();
                break;
            }
        case DragGestureHandler2D::Update::Type::End:
        case DragGestureHandler2D::Update::Type::Cancel:
            {
                auto renderer = marqueeRenderer.lock();
                if (renderer && canvas) {
                    auto toolsNode = canvas->ToolsNode();
                    if (toolsNode) {
                        toolsNode->Remove(*renderer);
                    }
                }
                marqueeRenderer.reset();
                break;
            }
        }
    };
}
