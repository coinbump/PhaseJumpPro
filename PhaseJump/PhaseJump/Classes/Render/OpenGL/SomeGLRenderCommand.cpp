#include "SomeGLRenderCommand.h"
#include "GLRenderEngine.h"
#include <stdio.h>

using namespace PJ;
using namespace std;

SP<SomeGLRenderCommand> GLRenderCommands::SetViewportRender(Vector2Int size) {
    return MAKE<GLRenderCommand<Vector2Int>>(size, [](Vector2Int& core, GLRenderEngine& engine) {
        engine.SetViewport(0, 0, core.x, core.y);
    });
}

SP<SomeGLRenderCommand> GLRenderCommands::ProjectionMatrixLoadOrthographic(Vector2 size) {
    return MAKE<GLRenderCommand<Vector2>>(size, [](Vector2& core, GLRenderEngine& engine) {
        engine.projectionMatrix.LoadOrthographic(0, core.x, 0, core.y, 1, -1);
    });
}

SP<SomeGLRenderCommand> GLRenderCommands::LoadTranslate(Vector3 value) {
    return MAKE<GLRenderCommand<Vector3>>(value, [](Vector3& core, GLRenderEngine& engine) {
        engine.viewMatrix.LoadTranslate(core);
    });
}
