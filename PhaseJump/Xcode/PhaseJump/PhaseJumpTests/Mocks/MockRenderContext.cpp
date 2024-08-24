#include "MockRenderContext.h"
#include "SomeRenderer.h"

using namespace PJ;
using namespace PJTest;

void MockRenderContext::Render(SomeRenderer& renderer, RenderContextModel model)
{
    Add(renderHistory, model);

    renderer.RenderInto(model);
}
