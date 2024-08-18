#include "SomeRenderCommandModel.h"

using namespace std;
using namespace PJ;

UP<RenderCommandModel<Vector2>> RenderCommandModels::ProjectionMatrixLoadOrthographic(Vector2 size
) {
    return std::make_unique<RenderCommandModel<Vector2>>(
        RenderCommandId::ProjectionMatrixLoadOrthographic, size
    );
}

UP<RenderCommandModel<Vector3>> RenderCommandModels::LoadTranslate(Vector3 value) {
    return std::make_unique<RenderCommandModel<Vector3>>(RenderCommandId::LoadTranslate, value);
}
