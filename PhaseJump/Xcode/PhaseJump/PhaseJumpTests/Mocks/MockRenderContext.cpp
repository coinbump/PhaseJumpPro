#include "MockRenderContext.h"
#include "SomeRenderer.h"

using namespace PJ;
using namespace PJTest;

void MockRenderContext::Render(SomeRenderer& renderer, RenderIntoModel model)
{
    renderHistory.Add(model);

    renderer.RenderInto(model);
}
