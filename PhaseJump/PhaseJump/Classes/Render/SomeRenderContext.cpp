#include "SomeRenderContext.h"
#include "RenderIntoModel.h"
#include "SomeRenderer.h"

using namespace PJ;

void SomeRenderContext::Render(SomeRenderer& renderer, RenderIntoModel model) {
    renderer.RenderInto(model);
}
