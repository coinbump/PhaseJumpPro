#include "SomeGLRenderCommand.h"
#include "GLRenderEngine.h"
#include <stdio.h>

using namespace PJ;
using namespace std;

SP<SomeGLRenderCommand> GLRenderCommands::SetViewportRender(Vector2Int size) {
    return MAKE<GLRenderCommand<Vector2Int>>(size, [](auto& command, GLRenderEngine& engine) {
        engine.SetViewport(0, 0, command.core.x, command.core.y);
    });
}

SP<SomeGLRenderCommand> GLRenderCommands::ProjectionMatrixLoadOrthographic(Vector2 size) {
    return MAKE<GLRenderCommand<Vector2>>(size, [](auto& command, GLRenderEngine& engine) {
        // TODO: are these 1, -1 values correct
        engine.projectionMatrix.LoadOrthographic(0, command.core.x, 0, command.core.y, 1, -1);
    });
}

SP<SomeGLRenderCommand> GLRenderCommands::LoadTranslate(Vector3 value) {
    return MAKE<GLRenderCommand<Vector3>>(value, [](auto& command, GLRenderEngine& engine) {
        engine.viewMatrix.LoadTranslate(command.core);
    });
}
