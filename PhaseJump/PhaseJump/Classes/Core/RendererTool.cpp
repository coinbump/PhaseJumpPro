#include "RendererTool.h"
#include "RenderMaterial.h"
#include "SomeTexture.h"
#include "SpriteRenderer.h"
#include "WorldNode.h"
#include <optional>

using namespace std;
using namespace PJ;

RendererTool::RendererTool(WorldNode& node) {
    renderer = node.GetComponent<SomeRenderer>();
}

std::optional<Vector3> RendererTool::WorldSize() const {
    if (renderer) {
        return renderer->WorldSize();
    }

    return std::nullopt;
}

void RendererTool::SetColor(Color color) {
    if (renderer) {
        return renderer->SetColor(color);
    }
}
