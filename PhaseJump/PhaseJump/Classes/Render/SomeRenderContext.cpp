#include "SomeRenderContext.h"
#include "SomeRenderer.h"
#include "RenderIntoModel.h"

using namespace PJ;

void SomeRenderContext::Render(SomeRenderer& renderer, RenderIntoModel model)
{
    renderer.RenderInto(model);
}
