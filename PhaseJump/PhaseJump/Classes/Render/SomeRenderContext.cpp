#include "SomeRenderContext.h"
#include "RenderContextModel.h"
#include "SomeRenderer.h"

using namespace PJ;

void SomeRenderContext::Render(SomeRenderer& renderer, RenderContextModel model) {
    renderer.RenderInto(model);
}
