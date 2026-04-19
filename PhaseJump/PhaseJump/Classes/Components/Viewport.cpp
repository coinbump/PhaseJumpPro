#include "Viewport.h"
#include "BatchByMaterialRenderProcessor.h"
#include "RenderContextModel.h"
#include "SomeRenderContext.h"
#include "SomeRenderEngine.h"
#include "UIPlanner.h"
#include "World.h"
#include "WorldNode.h"
#include <format>

using namespace std;
using namespace PJ;

Viewport::Viewport() {
    type = CameraType::Viewport;

    // Assigns staggered z values to opaque render models so the depth test produces correct
    // parent-before-child ordering. Without this processor, every opaque model ends up at the
    // same depth and GL_LESS rejects children that overlap a parent.
    processingModel.Add(MAKE<BatchByMaterialRenderProcessor>());

    worldSize.SetOnValueChangeFunc(SetOnValueChangeFuncType::None, [this](Vector2 value) {
        ResizeBuffer(value);
    });

    PlanUIFunc planUIFunc = [this](auto args) {
        args.planner.LabelText([this]() {
            auto size = worldSize.Value();
            return UIPlanner::LabelTextConfig{ .label = "World Size",
                                               .text = std::format("{}, {}", size.x, size.y) };
        });
        args.planner.LabelText([this]() {
            String text = "(none)";
            if (renderContext) {
                auto size = renderContext->Size();
                text = std::format("{}, {}", size.x, size.y);
            }
            return UIPlanner::LabelTextConfig{ .label = "Buffer Size", .text = text };
        });

        args.planner.LabelText([this]() {
            if (renderContext) {
                return UIPlanner::LabelTextConfig{ .label = "Texture ID",
                                                   .text = MakeString(renderContext->RenderId()) };
            }

            return UIPlanner::LabelTextConfig{ .label = "Texture ID: MISSING" };
        });

        args.planner.Image([this]() {
            RenderItemId id = renderContext ? renderContext->RenderId() : 0;
            auto size = worldSize.Value() * 0.5f;
            return UIPlanner::ImageConfig{ .imageId = id, .size = size };
        });
    };
    Override(planUIFuncs[UIContextId::Inspector], planUIFunc);
}

void Viewport::Awake() {
    Base::Awake();

    ResizeBuffer(worldSize.Value());
}

void Viewport::ResizeBuffer(Vector2 size) {
    GUARD(size.x > 0 && size.y > 0)
    GUARD(owner)

    auto world = owner->World();
    GUARD(world && world->renderEngine)

    Vector2Int pixelSize((int)size.x, (int)size.y);

    if (renderContext) {
        renderContext->Resize(pixelSize);
    } else {
        renderContext = world->renderEngine->MakeTextureBuffer();
        GUARD(renderContext)
        renderContext->Build(pixelSize);
    }
}

// Converts buffer pixel coordinates (top-left origin, Y-down) to world cartesian
// coordinates (Y-up), with the buffer centered on the owner's world position.
Vector3 Viewport::ContextToWorld(Vector3 position) const {
    GUARDR(owner, {})
    auto ownerPos = owner->transform.WorldPosition();
    auto size = worldSize.Value();
    return Vector3(
        ownerPos.x + position.x - size.x * 0.5f, ownerPos.y + size.y * 0.5f - position.y, 0
    );
}

Vector3 Viewport::WorldToContext(Vector3 worldPos) const {
    GUARDR(owner, {})
    auto ownerPos = owner->transform.WorldPosition();
    auto size = worldSize.Value();
    return Vector3(
        worldPos.x - ownerPos.x + size.x * 0.5f, size.y * 0.5f - (worldPos.y - ownerPos.y), 0
    );
}

void Viewport::RenderStart(SomeRenderContext* context) {
    Base::RenderStart(context);

    GUARD(owner)

    auto renderContext = dynamic_cast<RenderContext*>(context);
    GUARD(renderContext)

    auto size = worldSize.Value();
    GUARD(size.x > 0 && size.y > 0)

    renderContext->renderEngine.ProjectionMatrixLoadOrthographic(size);

    auto worldPosition = owner->transform.WorldPosition();
    Vector3 translate(size.x * 0.5f - worldPosition.x, size.y * 0.5f - worldPosition.y, 0);
    renderContext->renderEngine.LoadTranslate(translate);
}

Vector2 Viewport::ContextToScreen(Vector3 position) {
    auto worldPos = ContextToWorld(position);

    GUARDR(owner, {})
    auto world = owner->World();
    GUARDR(world, {})
    auto mainCamera = world->MainCamera();
    GUARDR(mainCamera, {})

    return mainCamera->ContextToScreen(worldPos);
}

Vector3 Viewport::ScreenToContext(Vector2 position) {
    GUARDR(owner, {})
    auto world = owner->World();
    GUARDR(world, {})
    auto mainCamera = world->MainCamera();
    GUARDR(mainCamera, {})

    auto worldPos = mainCamera->ScreenToContext(position);
    return WorldToContext(worldPos);
}
