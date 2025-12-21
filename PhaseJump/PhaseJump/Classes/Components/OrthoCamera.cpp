#include "OrthoCamera.h"
#include "Matrix4x4.h"
#include "Mesh.h"
#include "RenderContextModel.h"
#include "SomeRenderCommandModel.h"
#include "SomeRenderEngine.h"
#include "World.h"

using namespace std;
using namespace PJ;

OrthoCamera::~OrthoCamera() {}

Vector2 OrthoCamera::RenderContextSize() const {
    GUARDR(owner, {})
    auto world = owner->World();

    GUARDR(world && world->renderContext, {})

    auto contextSize = world->renderContext->Size();
    return contextSize;
}

Vector2 OrthoCamera::RenderContextPixelSize() const {
    GUARDR(owner, {})
    auto world = owner->World();

    GUARDR(world && world->renderContext, {})

    auto contextPixelSize = owner->World()->renderContext->PixelSize();
    return contextPixelSize;
}

Vector2 OrthoCamera::RenderContextExtents() const {
    return RenderContextSize() / 2.0f;
}

Vector2 OrthoCamera::CameraExtents() const {
    auto contextSize = RenderContextPixelSize();
    Vector2 result(contextSize.x / 2.0f, contextSize.y / 2.0f);
    float heightToWidth = result.x / result.y;

    if (halfHeight && *halfHeight > 0) {
        result = Vector2(*halfHeight * heightToWidth, *halfHeight);
    }

    return result;
}

Vector2 OrthoCamera::CameraSize() const {
    return CameraExtents() * 2.0f;
}

Vector2 OrthoCamera::WorldToScreenScale() const {
    auto cameraSize = CameraSize();
    auto contextSize = RenderContextSize();

    // Prevent divide by zero
    GUARDR(cameraSize != Vector2{} && contextSize != Vector2{}, Vector2::one)

    Vector2 result = contextSize / cameraSize;
    return result;
}

Vector2 OrthoCamera::WorldToScreen(Vector3 position) {
    auto contextExtents = RenderContextExtents();
    auto cameraPosition = owner->transform.WorldPosition();

    auto scale = WorldToScreenScale();
    position *= scale;
    cameraPosition *= scale;

    // FUTURE: update to support different coordinate systems
    Vector2 screenPosition(
        position.x - cameraPosition.x + contextExtents.x,
        cameraPosition.y + contextExtents.y - position.y
    );

    return screenPosition;
}

Vector3 OrthoCamera::ScreenToWorld(Vector2 position) {
    auto contextExtents = RenderContextExtents();
    auto cameraPosition = owner->transform.WorldPosition();

    auto worldToScreenScale = WorldToScreenScale();
    GUARDR(worldToScreenScale.x != 0 && worldToScreenScale.y != 0, {})
    auto screenToWorldScale = Vector2::one / worldToScreenScale;

    Vector2 screenCartesian =
        Vector2(position.x + contextExtents.x * vecLeft, -position.y + contextExtents.y * vecUp);
    screenCartesian *= screenToWorldScale;

    // FUTURE: update to support different coordinate systems
    Vector3 worldPosition(
        screenCartesian.x + cameraPosition.x, screenCartesian.y + cameraPosition.y, 0
    );

    return worldPosition;
}

void OrthoCamera::RenderStart(SomeRenderContext* context) {
    Base::RenderStart(context);

    GUARD(owner)

    auto renderContext = dynamic_cast<RenderContext*>(context);
    GUARD(renderContext)

    auto worldPosition = owner->transform.WorldPosition();

    auto cameraExtents = CameraExtents();
    renderContext->renderEngine.ProjectionMatrixLoadOrthographic(cameraExtents * 2.0f);

    // Translate to standard position
    Vector3 translate(cameraExtents.x, cameraExtents.y, 0);

    // Translate to camera position
    translate.x -= worldPosition.x;
    translate.y -= worldPosition.y;

    renderContext->renderEngine.LoadTranslate(translate);
}

bool OrthoCamera::IsCulled(Mesh const& mesh) {
    // TODO: needs unit tests
    auto halfSize = CameraExtents();

    bool result = false;

    Vector3 cameraExtents(halfSize.x, halfSize.y, 0);
    Bounds cameraBounds2D({}, cameraExtents);
    Vector3 meshCenter2D(mesh.GetBounds().center.x, mesh.GetBounds().center.y, 0);
    Vector3 meshExtents2D(mesh.GetBounds().extents.x, mesh.GetBounds().extents.y, 0);
    Bounds meshBounds2D(meshCenter2D, meshExtents2D);

    result = !cameraBounds2D.TestCollide(meshBounds2D);

    return result;
}
