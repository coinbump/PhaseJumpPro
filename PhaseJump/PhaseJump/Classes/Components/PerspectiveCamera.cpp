#include "PerspectiveCamera.h"
#include "Matrix4x4.h"
#include "Mesh.h"
#include "RenderContextModel.h"
#include "SomeRenderCommandModel.h"
#include "SomeRenderEngine.h"
#include "World.h"

using namespace std;
using namespace PJ;

PerspectiveCamera::~PerspectiveCamera() {}

Vector2 PerspectiveCamera::RenderContextPixelSize() const {
    GUARDR(owner, {})
    auto world = owner->World();

    GUARDR(world && world->renderContext, {})

    return world->renderContext->PixelSize();
}

float PerspectiveCamera::Aspect() const {
    auto pixelSize = RenderContextPixelSize();
    GUARDR(pixelSize.y > 0, 1.0f)
    return pixelSize.x / pixelSize.y;
}

Vector2 PerspectiveCamera::ContextToScreen(Vector3 position) {
    // FUTURE: project through camera matrices for accurate screen mapping
    return Vector2(position.x, position.y);
}

Vector3 PerspectiveCamera::ScreenToContext(Vector2 position) {
    // FUTURE: unproject to a camera ray for accurate picking
    return Vector3(position.x, position.y, 0);
}

void PerspectiveCamera::RenderStart(SomeRenderContext* context) {
    Base::RenderStart(context);

    GUARD(owner)

    auto renderContext = dynamic_cast<RenderContext*>(context);
    GUARD(renderContext)

    auto worldPosition = owner->transform.WorldPosition();

    renderContext->renderEngine.ProjectionMatrixLoadPerspective(
        fov.Radians(), Aspect(), nearClip, farClip
    );

    // Translate world so camera ends up at the origin. Camera looks down -Z
    Vector3 translate(-worldPosition.x, -worldPosition.y, -worldPosition.z);
    renderContext->renderEngine.LoadTranslate(translate);
}
