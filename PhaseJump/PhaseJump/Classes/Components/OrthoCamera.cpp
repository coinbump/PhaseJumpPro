#include "OrthoCamera.h"
#include "Matrix4x4.h"
#include "RenderContextModel.h"
#include "SomeRenderCommandModel.h"
#include "SomeRenderEngine.h"
#include "World.h"

using namespace std;
using namespace PJ;

OrthoCamera::~OrthoCamera() {}

// TODO: what should this be named?
Matrix4x4 OrthoCamera::Matrix() {
    Matrix4x4 result;

    GUARDR(owner, result)
    auto owner = this->owner;

    auto worldPosition = owner->transform.WorldPosition();

    result.LoadTranslate(Vector3(-worldPosition.x, -worldPosition.y, 0));

    return result;
}

// Note: this assumes the ortho camera matches the screen exactly
// FUTURE: support ortho camera size
Vector2 OrthoCamera::WorldToScreen(Vector3 position) {
    Vector2 result;

    GUARDR(owner, result)
    auto owner = this->owner;

    auto world = owner->World();
    GUARDR(world, result)

    auto renderContext = world->renderContext;
    if (nullptr == renderContext) {
        return result;
    }

    auto size = renderContext->Size();
    auto cameraPosition = owner->transform.WorldPosition();

    Vector2 screenPosition(
        position.x - cameraPosition.x + size.x / 2.0f, cameraPosition.y + size.y / 2.0f - position.y
    );
    return screenPosition;
}

// Note: this assumes the ortho camera matches the screen exactly
// FUTURE: support ortho camera size
Vector3 OrthoCamera::ScreenToWorld(Vector2 position) {
    Vector3 result;

    GUARDR(owner, result)
    auto owner = this->owner;

    auto world = owner->World();
    GUARDR(world, result)

    auto renderContext = world->renderContext;
    GUARDR(renderContext, result)

    auto size = renderContext->Size();
    auto cameraPosition = owner->transform.WorldPosition();

    Vector3 worldPosition(
        position.x - cameraPosition.x - size.x / 2.0f,
        size.y / 2.0f - position.y - cameraPosition.y, 0
    );
    return worldPosition;
}

void OrthoCamera::PreRender(RenderContextModel const& contextModel) {
    Base::PreRender(contextModel);

    auto renderContext = contextModel.renderContext;
    GUARD(renderContext)

    GUARD(owner)
    auto owner = this->owner;

    auto worldPosition = owner->transform.WorldPosition();

    GUARD(renderContext);
    auto size = renderContext->PixelSize();
    Vector2 orthoSize{ (float)size.x, (float)size.y };

    renderContext->renderEngine->ProjectionMatrixLoadOrthographic(orthoSize);

    // Translate to standard position
    Vector3 translate(orthoSize.x / 2.0f, orthoSize.y / 2.0f, 0);

    // Translate to camera position
    translate.x += worldPosition.x;
    translate.y += worldPosition.y;

    renderContext->renderEngine->LoadTranslate(translate);
}
