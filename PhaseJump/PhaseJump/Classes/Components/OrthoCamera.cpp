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
    GUARDR(owner && owner->World() && owner->World()->renderContext, {})

    // FUTURE: re-evaluate Size vs PixelSize with hi-DP screens
    auto contextSize = owner->World()->renderContext->PixelSize();
    return Vector2(contextSize.x, contextSize.y);
}

Vector2 OrthoCamera::RenderContextExtents() const {
    return RenderContextSize() / 2.0f;
}

Vector2 OrthoCamera::CameraExtents() const {
    auto contextSize = RenderContextSize();
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

void OrthoCamera::PreRender(RenderContextModel const& contextModel) {
    Base::PreRender(contextModel);

    GUARD(owner)

    auto renderContext = contextModel.renderContext;
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
    auto halfSize = CameraExtents();

    bool result = false;

    switch (frustrum) {
    case Frustrum::None:
        {
            Vector3 cameraExtents(halfSize.x, halfSize.y, 0);
            Bounds cameraBounds2D({}, cameraExtents);
            Vector3 meshCenter2D(mesh.GetBounds().center.x, mesh.GetBounds().center.y, 0);
            Vector3 meshExtents2D(mesh.GetBounds().extents.x, mesh.GetBounds().extents.y, 0);
            Bounds meshBounds2D(meshCenter2D, meshExtents2D);

            result = !cameraBounds2D.TestCollide(meshBounds2D);
            break;
        }
    case Frustrum::Enabled:
        {
            // TODO: this doesn't make sense. OpenGL always has a frustrum. Rethink this (is this
            // just for debugging culls?)
            Vector3 frustrumExtents(halfSize.x, halfSize.y, abs(farClip - nearClip) / 2.0f);
            Vector3 frustrumCenter(0, 0, nearClip + frustrumExtents.z * Vector3::forward.z);

            Bounds frustrumBounds(frustrumCenter, frustrumExtents);
            Bounds meshBounds = mesh.GetBounds();
            result = !frustrumBounds.TestCollide(meshBounds);
            break;
        }
    }

    return result;
}
