#include "View2D.h"
#include "HoverGestureHandler.h"
#include "QuickBuilder.h"
#include "UIPlanner.h"
#include "ViewBuilder.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

View2D::View2D() {
    Updatable::OnUpdateFunc onUpdateFunc = [=, this](Updatable& updatable, TimeSlice time) {
        RebuildIfNeeded();
        LayoutIfNeeded();
        return FinishType::Continue;
    };
    GetUpdatable().onUpdateFunc = onUpdateFunc;

    AddSignalHandler<AddChildNodeEvent>({ .id = SignalId::ChildNodeAdd,
                                          .func = [this](auto& _component, auto& event) {
                                              auto view =
                                                  event.node.template TypeComponent<View2D>();
                                              GUARD(view)

                                              // Top level views don't affect layout
                                              GUARD(!view->IsTopLevel())

                                              SetNeedsLayout();
                                          } });
    AddSignalHandler<RemoveChildNodeEvent>({ .id = SignalId::ChildNodeRemove,
                                             .func = [this](auto& _component, auto& event) {
                                                 auto view =
                                                     event.node.template TypeComponent<View2D>();
                                                 GUARD(view)

                                                 // Top level views don't affect layout
                                                 GUARD(!view->IsTopLevel())

                                                 SetNeedsLayout();
                                             } });

    AddSignalHandler<HoverUIEvent>({ .id = SignalId::Hover, .func = [](auto& owner, auto& event) {
                                        static_cast<This*>(&owner)->SetIsHovering(event.isHovering);
                                    } });

    PlanUIFunc planUIFunc = [this](auto args) {
        auto& planner = args.planner;

        args.planner.Text([this]() {
            String frameString = std::format(
                "Origin: {}, {} \tSize: {}, {}", frame.origin.x, frame.origin.y, frame.size.x,
                frame.size.y
            );
            return UIPlanner::TextConfig{ .label = "Frame", .text = frameString };
        });

        planner.InputFloat({ .label = "Fixed Width",
                             .binding = { [this]() { return FixedWidth() ? *FixedWidth() : 0; },
                                          [this](auto& value) {
                                              SetFixedSize(
                                                  value > 0 ? value : std::optional<float>{},
                                                  FixedHeight()
                                              );
                                          } } });
        planner.InputFloat({ .label = "Fixed Height",
                             .binding = { [this]() { return FixedHeight() ? *FixedHeight() : 0; },
                                          [this](auto& value) {
                                              SetFixedSize(
                                                  FixedWidth(),
                                                  value > 0 ? value : std::optional<float>{}
                                              );
                                          } } });

        planner.InputFloat(
            { .label = "Max Width",
              .binding = { [this]() { return MaxWidth() ? *MaxWidth() : 0; },
                           [this](auto& value) {
                               SetMaxSize(value > 0 ? value : std::optional<float>{}, MaxHeight());
                           } } }
        );
        planner.InputFloat(
            { .label = "Max Height",
              .binding = { [this]() { return MaxHeight() ? *MaxHeight() : 0; },
                           [this](auto& value) {
                               SetMaxSize(MaxWidth(), value > 0 ? value : std::optional<float>{});
                           } } }
        );

        planner.InputBool({ .label = "Ideal Width",
                            .binding = { [this]() { return IsIdealWidth(); },
                                         [this](auto& value) {
                                             SetIsIdealSize(value, IsIdealHeight());
                                         } } });
        planner.InputBool({ .label = "Ideal Height",
                            { [this]() { return IsIdealHeight(); },
                              [this](auto& value) { SetIsIdealSize(IsIdealWidth(), value); } } });

        if (layout) {
            String layoutLabel = std::format("Layout: {}", layout->TypeName());
            planner.Text({ .text = layoutLabel });

            auto planUIFunc = layout->GetPlanUIFunc(args.context);
            if (planUIFunc) {
                planUIFunc({ .layout = *layout, .context = args.context, .planner = planner });
            }
        }

        // Don't rebuild in immediate mode render loop, it will flash
        planner.Button({ .label = "Rebuild", .func = [this]() { SetNeedsRebuild(); } });
    };
    Override(planUIFuncs[UIContextId::Inspector], planUIFunc);
}

void View2D::Start() {
    Base::Start();

    // Run in start, not Awake, so everything is in place
    UpdateFrameComponents();
    LayoutIfNeeded();
}

View2D* View2D::ParentView() const {
    GUARDR(owner && owner->Parent(), {})

    return owner->Parent()->TypeComponent<View2D>();
}

View2D* View2D::RootView() const {
    View2D* result = const_cast<View2D*>(this);

    auto parent = ParentView();
    while (parent) {
        result = parent;
        parent = parent->ParentView();
    }

    return result;
}

View2D* View2D::FirstChildView() const {
    GUARDR(owner, {})

    for (auto& child : owner->Children()) {
        auto view = child->TypeComponent<View2D>();
        if (view) {
            return view;
        }
    }

    return {};
}

VectorList<View2D*> View2D::ChildViews() const {
    GUARDR(owner, {})

    VectorList<View2D*> result;

    for (auto& child : owner->Children()) {
        // Only 1 child view component supported per node
        auto view = child->TypeComponent<View2D>();
        if (view) {
            result.push_back(view);
        }
    }

    return result;
}

bool View2D::TestViewPositionHit(Vector2 viewPosition) const {
    GUARDR(viewPosition.x >= 0 && viewPosition.y >= 0, false)
    GUARDR(viewPosition.x < frame.size.x && viewPosition.y < frame.size.y, false)

    return true;
}

Vector2 View2D::ToViewPosition(LayoutAnchor2D viewPoint) const {
    return { frame.size.x * viewPoint.x, frame.size.y * viewPoint.y };
}

Vector3 View2D::TopLeftWorldPosition() const {
    return LocalToWorld(TopLeftLocalPosition());
}

Vector3 View2D::ViewToWorld(Vector2 viewPosition) const {
    return LocalToWorld(ViewToLocal(viewPosition));
}

Vector3 View2D::ViewToLocal(Vector2 viewPosition) const {
    auto local = viewPosition - frame.size / 2.0f;
    local.y *= vecDown;

    return local;
}

Vector2 View2D::WorldToView(Vector3 worldPosition) const {
    auto localPosition = WorldToLocal(worldPosition);
    return LocalToView(localPosition);
}

Vector2 View2D::LocalToView(Vector3 localPosition) const {
    auto viewX = localPosition.x + frame.size.x / 2.0f;
    auto viewY = frame.size.y / 2.0f - localPosition.y;

    return { viewX, viewY };
}

Vector3 View2D::TopLeftLocalPosition() const {
    Vector3 result{ frame.size.x / 2.0f * vecLeft, frame.size.y / 2.0f * vecUp,
                    owner->transform.LocalPosition().z };
    return result;
}

Vector3 View2D::ChildTopLeftLocalPosition(Rect childFrame) const {
    auto result = TopLeftLocalPosition();
    result.x += childFrame.origin.x * vecRight;
    result.y += childFrame.origin.y * vecDown;
    return result;
}

Vector3 View2D::ChildLocalPosition(Rect childFrame) const {
    auto result = ChildTopLeftLocalPosition(childFrame);
    result.x += childFrame.size.x / 2.0f * vecRight;
    result.y += childFrame.size.y / 2.0f * vecDown;
    return result;
}

void View2D::StartLayout() {
    std::function<void(View2D&)> prepareRecurse = [&, this](View2D& view) {
        if (!view.IsRoot()) {
            view.StartLayout();
        }

        for (auto& child : view.ChildViews()) {
            prepareRecurse(*child);
        }
    };

    if (IsRoot()) {
        prepareRecurse(*this);
    } else {
        if (layout) {
            layout->StartLayout();
        }
    }
}

void View2D::ApplyLayout() {
    std::function<void(View2D&)> layoutRecurse = [&, this](View2D& view) {
        auto& layout = view.layout;
        if (layout) {
            auto proxies = view.ChildViewProxies();
            layout->LayoutSubviews(view.Bounds().size, proxies);

            for (auto& child : view.ChildViews()) {
                layoutRecurse(*child);
            }
        }
    };

    auto layoutRoot = [this, layoutRecurse]() {
        auto childViews = ChildViews();

        for (int i = 0; i < childViews.size(); i++) {
            auto isTopLevel = i > 0;
            auto child = childViews[i];
            GUARD_CONTINUE(child->IsEnabled())

            // The first child view's layout determines its size
            // Top level views have a fixed size, which must be calculated when the view is
            // presented
            if (child->layout && !isTopLevel) {
                ViewSizeProposal proposal({ .width = frame.size.x, .height = frame.size.y });

                auto proxies = child->ChildViewProxies();
                auto viewSize = child->layout->ViewSize(proposal, proxies);

                // Pin unbounded frame values to root view size
                if (viewSize.x == FloatMath::maxValue) {
                    viewSize.x = frame.size.x;
                }
                if (viewSize.y == FloatMath::maxValue) {
                    viewSize.y = frame.size.y;
                }

                Rect childBounds;
                childBounds.size = viewSize;
                child->SetFrame(childBounds);
            }

            child->owner->SetLocalPosition(
                ViewToLocal(child->frame.origin + child->frame.size / 2.0f)
            );

            layoutRecurse(*child);
        }
    };

    // Root view has a fixed size and centers its first child
    // Root provides the bounds for all descendants
    if (IsRoot()) {
        StartLayout();

        layoutRoot();
    } else {
        auto root = RootView();
        GUARD(root)

        root->ApplyLayout();
    }

    // Wait until the end to set this flag, because we are using SetFrame, which invalidates it
    RootView()->needsLayout = false;
}

void View2D::LayoutIfNeeded() {
    auto root = RootView();
    GUARD(root)

    GUARD(root->needsLayout);
    root->ApplyLayout();
}

void View2D::SetNeedsLayout() {
    auto root = RootView();
    GUARD(root)

    // Child view frame change requires rebuild from root
    root->needsLayout = true;
}

void View2D::SetLayout(UP<SomeViewLayout>& value) {
    if (value) {
        value->SetOwner(this);
    }
    layout = std::move(value);
    SetNeedsLayout();
}

VectorList<ViewProxy> View2D::ChildViewProxies() const {
    VectorList<ViewProxy> result;

    auto childViews = ChildViews();
    for (auto& child : childViews) {
        GUARD_CONTINUE(child->IsEnabled())

        ViewProxy proxy = child->MakeViewProxy();
        result.push_back(proxy);
    }

    return result;
}

ViewProxy View2D::MakeViewProxy() {
    ViewProxy proxy(this);

    proxy.viewSizeFunc = [this](auto& proxy, auto proposal) {
        if (layout) {
            auto proxies = ChildViewProxies();
            auto viewSize = layout->ViewSize(proposal, proxies);
            return viewSize;
        } else {
            // Default: views are unbounded
            Vector2 result{ FloatMath::maxValue, FloatMath::maxValue };

            bool isIdealWidth = IsIdealWidth();
            bool isIdealHeight = IsIdealHeight();
            if (isIdealWidth || isIdealHeight) {
                auto idealSize = IdealSize(proposal);

                if (isIdealWidth && idealSize.width) {
                    result.x = *idealSize.width;
                }
                if (isIdealHeight && idealSize.height) {
                    result.y = *idealSize.height;
                }
            }

            auto fixedWidth = FixedWidth();
            auto fixedHeight = FixedHeight();

            if (fixedWidth) {
                result.x = *fixedWidth;
            }
            if (fixedHeight) {
                result.y = *fixedHeight;
            }

            return result;
        }
    };
    proxy.placeViewFunc = ViewProxy::MakePlaceViewFunc(*this);

    return proxy;
}

Vector2 View2D::WithPositionConstraints(Vector2 origin) const {
    auto result = origin;

    auto positionX = constraints.Value<float>(UIConstraintId::PositionX);
    auto positionY = constraints.Value<float>(UIConstraintId::PositionY);
    if (positionX) {
        result.x = *positionX;
    }
    if (positionY) {
        result.y = *positionY;
    }

    auto offsetX = constraints.Value<float>(UIConstraintId::OffsetX);
    auto offsetY = constraints.Value<float>(UIConstraintId::OffsetY);
    if (offsetX) {
        result.x += *offsetX;
    }
    if (offsetY) {
        result.y += *offsetY;
    }

    return result;
}

Rect View2D::WithFrameConstraints(Rect frame) const {
    auto result = frame;

    frame.origin = WithPositionConstraints(frame.origin);
    frame.size = WithSizeConstraints(frame.size);

    return result;
}

Vector2 View2D::WithSizeConstraints(Vector2 size) const {
    auto result = size;

    /// Constraint: fixed width

    auto fixedWidth = FixedWidth();
    auto fixedHeight = FixedHeight();
    if (fixedWidth && fixedWidth != FloatMath::maxValue) {
        result.x = *fixedWidth;
    }
    if (fixedHeight && fixedHeight != FloatMath::maxValue) {
        result.y = *fixedHeight;
    }

    /// Constraint: max width

    auto maxWidth = MaxWidth();
    auto maxHeight = MaxHeight();
    if (maxWidth) {
        result.x = std::min(result.x, *maxWidth);
    }
    if (maxHeight) {
        result.y = std::min(result.y, *maxHeight);
    }

    return result;
}

void View2D::Rebuild() {
    GUARD(owner)
    GUARD(buildViewFunc)

    needsRebuild = false;

    owner->DestroyAllChildren();
    buildViewFunc(*this);

    SetNeedsLayout();
}

void View2D::SetNeedsRebuild() {
    needsRebuild = true;
}

void View2D::RebuildIfNeeded() {
    GUARD(needsRebuild);
    Rebuild();
}

VectorList<View2D*> View2D::TopLevelViews() {
    auto rootView = RootView();
    GUARDR(rootView, {})

    auto childViews = rootView->ChildViews();
    GUARDR(childViews.size() > 1, {})

    // Top level views are the views after the first view in the root
    childViews.erase(childViews.begin());
    return childViews;
}

void View2D::Present(PresentTopLevelViewConfig config) {
    GUARD(config.makeFrameFunc)
    GUARD(config.buildViewFunc)

    auto rootView = RootView();
    GUARD(rootView)

    ViewBuilder vb(*rootView->owner);
    config.buildViewFunc(vb);

    vb.QB().ModifyLatest<View2D>([&](auto& view) {
        auto frame = config.makeFrameFunc(*this, view);
        view.SetFrame(frame);
        view.isTopLevel = true;
    });
}

void View2D::Dismiss(View2D& view) {
    auto topLevelViews = TopLevelViews();
    GUARD(!IsEmpty(TopLevelViews()))

    if (Contains(topLevelViews, &view)) {
        auto* parent = view.owner->Parent();
        GUARD(parent)

        parent->Remove(*view.owner);
    }
}

void View2D::PopTopLevel() {
    auto topLevelViews = TopLevelViews();
    GUARD(!IsEmpty(TopLevelViews()))

    Dismiss(*topLevelViews[topLevelViews.size() - 1]);
}

void View2D::PopTo(View2D& view) {
    auto topLevelViews = TopLevelViews();
    GUARD(!IsEmpty(TopLevelViews()))

    for (auto i = topLevelViews.rbegin(); i != topLevelViews.rend(); i++) {
        auto& topLevelView = *i;
        if (topLevelView == &view) {
            return;
        }
        Dismiss(*topLevelView);
    }
}

Vector2 View2D::ToRootViewPosition(Vector2 viewPosition) {
    Vector2 result = viewPosition;

    auto parent = this;
    while (parent) {
        result += parent->frame.origin;
        parent = parent->ParentView();
    }

    return result;
}
