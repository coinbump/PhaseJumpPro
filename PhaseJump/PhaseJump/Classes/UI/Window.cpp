#include "Window.h"
#include "Animator.h"
#include "ButtonControl.h"
#include "Collider2D.h"
#include "ColorRenderer.h"
#include "Desktop.h"
#include "DragGestureHandler2D.h"
#include "DragHandler2D.h"
#include "EaseFunc.h"
#include "EllipseMeshBuilder.h"
#include "InterpolateFunc.h"
#include "LayoutInsets.h"
#include "PointerClickUIEvent.h"
#include "PolyFrameMeshBuilder.h"
#include "Polygon.h"
#include "QuickBuilder.h"
#include "RendererModel.h"
#include "RenderMaterial.h"
#include "SomeRenderContext.h"
#include "SpriteRenderer.h"
#include "Texture.h"
#include "View2D.h"
#include "ViewBuilder.h"
#include "Viewport.h"
#include "World.h"
#include "WorldNode.h"
#include <SDL3/SDL_mouse.h>

using namespace std;
using namespace PJ;

namespace {
    /// Returns the cursor id matching the resize handle's drag axis.
    String CursorIdForResize(Window::HResize h, Window::VResize v) {
        using HResize = Window::HResize;
        using VResize = Window::VResize;

        if (h == HResize::Left && v == VResize::Top)
            return CursorId::ResizeNorthwest;
        if (h == HResize::Right && v == VResize::Top)
            return CursorId::ResizeNortheast;
        if (h == HResize::Left && v == VResize::Bottom)
            return CursorId::ResizeSouthwest;
        if (h == HResize::Right && v == VResize::Bottom)
            return CursorId::ResizeSoutheast;
        if (h == HResize::Left && v == VResize::None)
            return CursorId::ResizeWest;
        if (h == HResize::Right && v == VResize::None)
            return CursorId::ResizeEast;
        if (h == HResize::None && v == VResize::Top)
            return CursorId::ResizeNorth;
        if (h == HResize::None && v == VResize::Bottom)
            return CursorId::ResizeSouth;
        return CursorId::Arrow;
    }
} // namespace

Window::Window() {
    worldSize.SetOnValueChangeFunc(SetOnValueChangeFuncType::None, [this](Vector2 value) {
        ApplyLayout(value);

        if (auto content = contentNode.lock()) {
            content->Signal(SignalId::ContainerResize, ContainerResizeEvent(value));
        }
    });

    isActive.SetOnValueChangeFunc(SetOnValueChangeFuncType::None, [this](bool value) {
        auto node = activateNode.lock();
        GUARD(node)
        node->Enable(!value);
    });
}

void Window::Awake() {
    Base::Awake();

    // Delegate drag to parent to drag entire window
    owner->AddComponentIfNeeded<DragHandler2D>();

    if (model.hasTitleBar) {
        auto _titleBarNode = MAKE<WorldNode>(WorldNode::Config{ .name = "Title Bar" });
        owner->Add(_titleBarNode);
        titleBarNode = _titleBarNode;

        QB(*_titleBarNode)
            .RectCollider()
            .With<ColorRenderer>(ColorRenderer::Config{ .color = Color::gray });

        auto& frameDrag = _titleBarNode->AddComponent<DragHandler2D>();
        frameDrag.dragTargetResolver = [this]() -> DragHandler* {
            GUARDR(owner, nullptr)
            return owner->GetComponent<DragHandler2D>();
        };

        auto _titleBarViewNode = MAKE<WorldNode>(WorldNode::Config{ .name = "Title Bar View" });
        _titleBarNode->Add(_titleBarViewNode);
        titleBarViewNode = _titleBarViewNode;

        float closeButtonSize = titleBarHeight * 0.7f;
        float titleBarPadding = titleBarHeight * 0.15f;

        auto addCircleButton = [closeButtonSize](
                                   auto& vb, String name, Color color,
                                   ButtonControl::OnPressFunc onPressFunc
                               ) {
            vb.QB()
                .And(name)
                .template With<ButtonControl>()
                .template ModifyLatest<ButtonControl>([=](auto& button) {
                    button.SetFixedSize(closeButtonSize, closeButtonSize);
                    button.onPressFunc = onPressFunc;
                })
                .template With<ColorRenderer>(ColorRenderer::Config{
                    .color = color, .worldSize = Vector2(closeButtonSize, closeButtonSize) })
                .template ModifyLatest<ColorRenderer>([](auto& renderer) {
                    renderer.EnableBlend(true);
                    renderer.core.model.SetBuildMeshFunc([](RendererModel const& model) {
                        EllipseMeshBuilder builder(model.WorldSize());
                        return builder.BuildMesh();
                    });
                })
                .RectCollider(Vector2(closeButtonSize, closeButtonSize))
                .Pop();
        };

        ViewBuilder vb(*_titleBarViewNode);
        vb.RootView(
            { worldSize.Value().x, titleBarHeight },
            [this, addCircleButton, titleBarPadding](auto& vb) {
                auto padScope = vb.BeginPad({ .insets = LayoutInsets::Uniform(titleBarPadding) });
                auto hStackScope = vb.BeginHStack({ .spacing = 8 });

                if (model.hasCloseButton) {
                    addCircleButton(vb, "Close Button", Color::red, [this](ButtonControl&) {
                        GUARD(closeFunc)
                        closeFunc(*this);
                    });
                }

                if (model.hasMaximizeButton) {
                    addCircleButton(vb, "Maximize Button", Color::green, [this](ButtonControl&) {
                        ToggleMaximize();
                    });
                }

                vb.Spacer();
            }
        );
    }

    // Content: viewport camera + sprite that displays the viewport's backing texture.
    auto _contentNode = MAKE<WorldNode>(WorldNode::Config{ .name = "Content" });
    owner->Add(_contentNode);
    contentNode = _contentNode;

    auto viewport = _contentNode->AddComponentPtr<Viewport>();

    auto size = worldSize.Value();
    float effectiveTitleBarHeight = model.hasTitleBar ? titleBarHeight : 0.0f;
    auto contentHeight = size.y - effectiveTitleBarHeight;
    if (size.x > 0 && contentHeight > 0) {
        viewport->worldSize = Vector2(size.x, contentHeight);
    }

    SP<Texture> viewportTexture;
    if (viewport->renderContext) {
        viewportTexture = viewport->renderContext->GetTexture();
    }

    _contentNode->AddComponent<SpriteRenderer>(SpriteRenderer::Config{ .texture = viewportTexture }
    );

    if (model.hasFrame) {
        auto _frameOverlayNode = MAKE<WorldNode>(WorldNode::Config{ .name = "Frame Overlay" });
        owner->Add(_frameOverlayNode);
        frameOverlayNode = _frameOverlayNode;

        auto& frameOverlayRenderer = _frameOverlayNode->AddComponent<ColorRenderer>(
            ColorRenderer::Config{ .color = Color::black }
        );
        frameOverlayRenderer.EnableBlend(true);
        frameOverlayRenderer.core.model.SetBuildMeshFunc([](RendererModel const& model) {
            PolyFrameMeshBuilder builder(PolyFrameMeshBuilder::Config{
                .poly = Polygon::MakeRect(model.WorldSize()), .strokeWidth = 2 });
            return builder.BuildMesh();
        });
    }

    resizeLeftEdgeNode = BuildResizeNode("ResizeLeftEdge", HResize::Left, VResize::None);
    resizeRightEdgeNode = BuildResizeNode("ResizeRightEdge", HResize::Right, VResize::None);
    resizeTopEdgeNode = BuildResizeNode("ResizeTopEdge", HResize::None, VResize::Top);
    resizeBottomEdgeNode = BuildResizeNode("ResizeBottomEdge", HResize::None, VResize::Bottom);
    resizeTopLeftCornerNode = BuildResizeNode("ResizeTopLeftCorner", HResize::Left, VResize::Top);
    resizeTopRightCornerNode =
        BuildResizeNode("ResizeTopRightCorner", HResize::Right, VResize::Top);
    resizeBottomLeftCornerNode =
        BuildResizeNode("ResizeBottomLeftCorner", HResize::Left, VResize::Bottom);
    resizeBottomRightCornerNode =
        BuildResizeNode("ResizeBottomRightCorner", HResize::Right, VResize::Bottom);

    auto _activateNode = MAKE<WorldNode>(WorldNode::Config{ .name = "Activate" });
    owner->Add(_activateNode);
    activateNode = _activateNode;

    QB(*_activateNode).RectCollider(Vector2(1, 1)).ModifyLatest<Collider2D>([this](auto& collider) {
        collider.template AddSignalHandler<PointerDownUIEvent>(
            { .id = SignalId::PointerDown,
              .func =
                  [this](auto& component, auto& event) {
                      GUARD(owner)
                      auto parent = owner->Parent();
                      GUARD(parent)
                      auto desktop = parent->TypeComponent<Desktop>();
                      GUARD(desktop)
                      desktop->Activate(*this);
                  } }
        );
    });

    _activateNode->Enable(!isActive.Value());

    ApplyLayout(size);

    if (buildContentFunc) {
        buildContentFunc(*_contentNode);
    }

    // Baseline content size for scale resize
    baseContentSize = Vector2(size.x, size.y - effectiveTitleBarHeight);
}

SP<WorldNode> Window::BuildResizeNode(String name, HResize hResize, VResize vResize) {
    auto node = MAKE<WorldNode>(WorldNode::Config{ .name = name });
    owner->Add(node);

    String cursorId = CursorIdForResize(hResize, vResize);
    auto savedCursor = MAKE<SDL_Cursor*>(nullptr);

    QB(*node)
        .RectCollider(Vector2(1, 1))
        .ModifyLatest<Collider2D>([this, cursorId, savedCursor](auto& collider) {
            collider.template AddSignalHandler<PointerEnterUIEvent>(
                { .id = SignalId::PointerEnter,
                  .func =
                      [this, cursorId, savedCursor](auto& component, auto& event) {
                          *savedCursor = SDL_GetCursor();
                          GUARD(owner && owner->World())
                          owner->World()->SetCursor(cursorId);
                      } }
            );

            collider.template AddSignalHandler<PointerExitUIEvent>(
                { .id = SignalId::PointerExit,
                  .func =
                      [savedCursor](auto& component, auto& event) {
                          GUARD(*savedCursor)
                          SDL_SetCursor(*savedCursor);
                          *savedCursor = nullptr;
                      } }
            );
        });

    struct DragState {
        Vector2 startSize;
        Vector3 startPosition;
    };

    auto state = MAKE<DragState>();

    auto& drag = node->AddComponent<DragGestureHandler2D>();
    drag.onDragGestureUpdateFunc = [this, hResize, vResize,
                                    state](DragGestureHandler2D::Update update) {
        using UpdateType = DragGestureHandler2D::Update::Type;
        GUARD(owner)

        switch (update.type) {
        case UpdateType::Start:
            state->startSize = worldSize.Value();
            state->startPosition = owner->transform.LocalPosition();
            break;
        case UpdateType::Update:
            {
                Vector2 translation = update.Translation();
                Vector2 newSize = state->startSize;
                Vector3 newPosition = state->startPosition;

                switch (hResize) {
                case HResize::Left:
                    newSize.x = std::max(state->startSize.x - translation.x, minWindowSize.x);
                    newPosition.x =
                        state->startPosition.x + (state->startSize.x - newSize.x) * 0.5f;
                    break;
                case HResize::Right:
                    newSize.x = std::max(state->startSize.x + translation.x, minWindowSize.x);
                    newPosition.x =
                        state->startPosition.x + (newSize.x - state->startSize.x) * 0.5f;
                    break;
                case HResize::None:
                    break;
                }

                switch (vResize) {
                case VResize::Top:
                    newSize.y = std::max(state->startSize.y + translation.y, minWindowSize.y);
                    newPosition.y =
                        state->startPosition.y + (newSize.y - state->startSize.y) * 0.5f;
                    break;
                case VResize::Bottom:
                    newSize.y = std::max(state->startSize.y - translation.y, minWindowSize.y);
                    newPosition.y =
                        state->startPosition.y + (state->startSize.y - newSize.y) * 0.5f;
                    break;
                case VResize::None:
                    break;
                }

                owner->transform.SetLocalPosition(newPosition);
                worldSize = newSize;
                break;
            }
        default:
            break;
        }
    };

    return node;
}

void Window::ApplyLayout(Vector2 size) {
    GUARD(size.x > 0 && size.y > 0)

    auto content = contentNode.lock();
    GUARD(content)

    float effectiveTitleBarHeight = model.hasTitleBar ? titleBarHeight : 0.0f;
    auto contentHeight = size.y - effectiveTitleBarHeight;
    GUARD(contentHeight > 0)

    // Frame sits at the top of the window (when present); content fills the rest below.
    auto halfWindowWidth = size.x * 0.5f;
    auto halfWindowHeight = size.y * 0.5f;
    auto halfFrameHeight = effectiveTitleBarHeight * 0.5f;
    auto halfContentHeight = contentHeight * 0.5f;

    if (auto frame = titleBarNode.lock()) {
        for (auto& component : frame->Components()) {
            auto worldSizeable = DCAST<WorldSizeable>(component);
            GUARD_CONTINUE(worldSizeable)

            worldSizeable->SetWorldSize(Vector3(size.x, titleBarHeight, 0));
        }

        frame->transform.SetLocalPosition(Vector3(0, halfWindowHeight - halfFrameHeight, 0));

        if (auto titleBarView = titleBarViewNode.lock()) {
            for (auto& component : titleBarView->Components()) {
                auto worldSizeable = DCAST<WorldSizeable>(component);
                GUARD_CONTINUE(worldSizeable)

                worldSizeable->SetWorldSize(Vector3(size.x, titleBarHeight, 0));
            }
        }
    }

    bool const canScale = contentResizeType == ContentResizeType::Scale && baseContentSize.x > 0 &&
                          baseContentSize.y > 0;

    if (canScale) {
        // Scale the content node
        float const scaleX = size.x / baseContentSize.x;
        float const scaleY = contentHeight / baseContentSize.y;
        content->transform.SetScale(Vector3(scaleX, scaleY, 1));
    } else {
        auto viewport = content->TypeComponent<Viewport>();
        if (viewport) {
            viewport->worldSize = Vector2(size.x, contentHeight);
        }

        auto contentRenderer = content->TypeComponent<SpriteRenderer>();
        if (contentRenderer) {
            contentRenderer->SetWorldSize(Vector3(size.x, contentHeight, 0));

            // Re-bind the viewport's texture — Resize rebuilds the backing texture, so the old one
            // the sprite was holding is now dead.
            if (viewport && viewport->renderContext) {
                auto texture = viewport->renderContext->GetTexture();
                auto material = contentRenderer->core.model.material;
                if (texture && material) {
                    material->SetTexture(texture);
                    contentRenderer->core.model.SetMeshNeedsBuild();
                }
            }
        }

        for (auto& child : content->Children()) {
            for (auto& component : child->Components()) {
                auto worldSizeable = DCAST<WorldSizeable>(component);
                GUARD_CONTINUE(worldSizeable)

                worldSizeable->SetWorldSize(Vector3(size.x, contentHeight, 0));
            }
        }
    }

    content->transform.SetLocalPosition(
        Vector3(0, halfWindowHeight - effectiveTitleBarHeight - halfContentHeight, 0)
    );

    if (auto overlay = frameOverlayNode.lock()) {
        for (auto& component : overlay->Components()) {
            auto worldSizeable = DCAST<WorldSizeable>(component);
            GUARD_CONTINUE(worldSizeable)

            worldSizeable->SetWorldSize(Vector3(size.x, size.y, 0));
        }
        overlay->transform.SetLocalPosition(Vector3(0, 0, 0));
    }

    auto layoutResize = [&](WP<WorldNode> const& weak, Vector2 resizeSize, Vector3 localPos) {
        auto node = weak.lock();
        GUARD(node)

        for (auto& component : node->Components()) {
            auto worldSizeable = DCAST<WorldSizeable>(component);
            GUARD_CONTINUE(worldSizeable)

            worldSizeable->SetWorldSize(Vector3(resizeSize.x, resizeSize.y, 0));
        }

        node->transform.SetLocalPosition(localPos);
    };

    auto halfFrameExtent = frameExtent * 0.5f;
    auto edgeLengthX = std::max(0.0f, size.x - 2 * frameExtent);
    auto edgeLengthY = std::max(0.0f, size.y - 2 * frameExtent);

    // Edges — inset by frameExtent on each end so they don't overlap the corner nodes
    layoutResize(
        resizeLeftEdgeNode, Vector2(frameExtent, edgeLengthY),
        Vector3(-halfWindowWidth + halfFrameExtent, 0, 0)
    );
    layoutResize(
        resizeRightEdgeNode, Vector2(frameExtent, edgeLengthY),
        Vector3(halfWindowWidth - halfFrameExtent, 0, 0)
    );
    layoutResize(
        resizeTopEdgeNode, Vector2(edgeLengthX, frameExtent),
        Vector3(0, halfWindowHeight - halfFrameExtent, 0)
    );
    layoutResize(
        resizeBottomEdgeNode, Vector2(edgeLengthX, frameExtent),
        Vector3(0, -halfWindowHeight + halfFrameExtent, 0)
    );

    // Corners
    layoutResize(
        resizeTopLeftCornerNode, Vector2(frameExtent, frameExtent),
        Vector3(-halfWindowWidth + halfFrameExtent, halfWindowHeight - halfFrameExtent, 0)
    );
    layoutResize(
        resizeTopRightCornerNode, Vector2(frameExtent, frameExtent),
        Vector3(halfWindowWidth - halfFrameExtent, halfWindowHeight - halfFrameExtent, 0)
    );
    layoutResize(
        resizeBottomLeftCornerNode, Vector2(frameExtent, frameExtent),
        Vector3(-halfWindowWidth + halfFrameExtent, -halfWindowHeight + halfFrameExtent, 0)
    );
    layoutResize(
        resizeBottomRightCornerNode, Vector2(frameExtent, frameExtent),
        Vector3(halfWindowWidth - halfFrameExtent, -halfWindowHeight + halfFrameExtent, 0)
    );

    // Activate overlay — covers the window interior, inset from the resize handles so they
    // remain hittable while the window is inactive.
    auto activateW = std::max(0.0f, size.x - 2 * frameExtent);
    auto activateH = std::max(0.0f, size.y - 2 * frameExtent);
    layoutResize(activateNode, Vector2(activateW, activateH), Vector3(0, 0, 0));
}

void Window::ToggleMaximize() {
    GUARD(owner)

    Vector2 startSize = worldSize.Value();
    Vector3 startPosition = owner->transform.LocalPosition();

    Vector2 endSize;
    Vector3 endPosition;

    if (state.Value() != State::Maximized) {
        defaultWorldSize = startSize;
        defaultLocalPosition = startPosition;

        Desktop* desktop{};
        auto parent = owner->Parent();
        if (parent) {
            desktop = parent->TypeComponent<Desktop>();
        }
        GUARD(desktop)

        state = State::Maximized;
        endSize = desktop->worldSize.Value();
        endPosition = Vector3(0, 0, startPosition.z);
    } else {
        state = State::Default;
        endSize = defaultWorldSize;
        endPosition = defaultLocalPosition;
    }

    // Cancel any in-progress maximize animation
    GetUpdatables().RemoveAll();

    constexpr float duration = 0.3f;

    auto sizeAnimator = NEW<Animator<Vector2>>(Animator<Vector2>::Config{
        .duration = duration,
        .interpolateFunc =
            InterpolateFuncs::Ease(InterpolateFuncs::Make(startSize, endSize), EaseFuncs::outCubed),
        .binding = [this](Vector2 value) { worldSize = value; } });
    GetUpdatables().Add(std::move(sizeAnimator));

    auto positionAnimator = NEW<Animator<Vector3>>(Animator<Vector3>::Config{
        .duration = duration,
        .interpolateFunc = InterpolateFuncs::Ease(
            InterpolateFuncs::Make(startPosition, endPosition), EaseFuncs::outCubed
        ),
        .binding = [this](Vector3 value) {
            GUARD(owner)
            owner->transform.SetLocalPosition(value);
        } });
    GetUpdatables().Add(std::move(positionAnimator));
}
