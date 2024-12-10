#include "View2D.h"
#include "SomeHoverGestureHandler.h"
#include "UIPlanner.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

View2D::View2D() {
    Updatable::OnUpdateFunc onUpdateFunc = [=, this](Updatable& updatable, TimeSlice time) {
        RebuildIfNeeded();
        LayoutIfNeeded();
        return FinishType::Continue;
    };
    updatable.onUpdateFunc = onUpdateFunc;

    signalFuncs[SignalId::AddChildNode] = [this](auto& _component, auto& event) {
        SetNeedsLayout();
    };
    signalFuncs[SignalId::RemoveChildNode] = [this](auto& _component, auto& event) {
        SetNeedsLayout();
    };

    signalFuncs[SignalId::Hover] = [](auto& owner, auto& signal) {
        auto event = static_cast<HoverUIEvent const*>(&signal);
        static_cast<This*>(&owner)->SetIsHovering(event->isHovering);
    };

    PlanUIFunc planUIFunc = [this](auto& component, String context, UIPlanner& planner) {
        planner.Text("Frame", [this]() {
            stringstream ss;
            ss << "Origin: " << MakeString(frame.origin.x) << ", " << MakeString(frame.origin.y);
            ss << "\t";
            ss << "Size: " << MakeString(frame.size.x) << ", " << MakeString(frame.size.y);

            return ss.str();
        });

        planner.InputFloat(
            "Fixed Width",
            { [this]() { return FixedWidth() ? *FixedWidth() : 0; },
              [this](auto& value) {
                  SetFixedSize(value > 0 ? value : std::optional<float>{}, FixedHeight());
              } }
        );
        planner.InputFloat(
            "Fixed Height",
            { [this]() { return FixedHeight() ? *FixedHeight() : 0; },
              [this](auto& value) {
                  SetFixedSize(FixedWidth(), value > 0 ? value : std::optional<float>{});
              } }
        );

        planner.InputFloat(
            "Max Width", { [this]() { return MaxWidth() ? *MaxWidth() : 0; },
                           [this](auto& value) {
                               SetMaxSize(value > 0 ? value : std::optional<float>{}, MaxHeight());
                           } }
        );
        planner.InputFloat(
            "Max Height", { [this]() { return MaxHeight() ? *MaxHeight() : 0; },
                            [this](auto& value) {
                                SetMaxSize(MaxWidth(), value > 0 ? value : std::optional<float>{});
                            } }
        );

        planner.InputBool(
            "Ideal Width", { [this]() { return IsIdealWidth(); },
                             [this](auto& value) { SetIsIdealSize(value, IsIdealHeight()); } }
        );
        planner.InputBool(
            "Ideal Height", { [this]() { return IsIdealHeight(); },
                              [this](auto& value) { SetIsIdealSize(IsIdealWidth(), value); } }
        );

        if (layout) {
            planner.Text("", [this]() {
                stringstream ss;
                ss << "Layout: " << layout->TypeName();
                return ss.str();
            });

            try {
                auto planUIFunc = layout->planUIFuncs.at(context);
                planUIFunc(*layout, context, planner);
            } catch (...) {}
        }

        // Don't rebuild in immediate mode render loop, it will flash
        planner.Button("Rebuild", [this]() { SetNeedsRebuild(); });
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

Vector3 View2D::TopLeftWorldPosition() const {
    // This result only makes sense if layout has already been applied to the view and its ancestors
    Vector3 worldPos = LocalToWorld({});

    Vector3 topLeft(
        worldPos.x + (WorldSize().x / 2.0f) * vecLeft, worldPos.y + (WorldSize().y / 2.0f) * vecUp,
        worldPos.z
    );
    return topLeft;
}

bool View2D::TestViewPositionHit(Vector2 viewPosition) const {
    GUARDR(viewPosition.x >= 0 && viewPosition.y >= 0, false)
    GUARDR(viewPosition.x < frame.size.x && viewPosition.y < frame.size.y, false)

    return true;
}

Vector2 View2D::ToViewPosition(ViewPoint viewPoint) const {
    return { frame.size.x * viewPoint.x, frame.size.y * viewPoint.y };
}

Vector3 View2D::ViewToWorldPosition(Vector2 viewPosition) const {
    auto topLeftWorldPosition = TopLeftWorldPosition();
    Vector3 result(
        topLeftWorldPosition.x + viewPosition.x * vecRight,
        topLeftWorldPosition.y + viewPosition.y * vecDown, topLeftWorldPosition.z
    );

    return result;
}

Vector3 View2D::ViewToLocalPosition(Vector2 viewPosition) const {
    return WorldToLocal(ViewToWorldPosition(viewPosition));
}

Vector2 View2D::WorldToViewPosition(Vector3 worldPosition) const {
    auto localPosition = WorldToLocal(worldPosition);
    return LocalToViewPosition(localPosition);
}

Vector2 View2D::LocalToViewPosition(Vector3 localPosition) const {
    auto viewX = localPosition.x + frame.size.x / 2.0f;
    auto viewY = frame.size.y / 2.0f - localPosition.y;

    return { viewX, viewY };
}

Vector3 View2D::TopLeftChildLocalPosition(Rect childFrame) const {
    auto parentFrame = frame;

    Vector3 topLeft(
        -parentFrame.size.x / 2.0f + childFrame.origin.x,
        (parentFrame.size.y / 2.0f) * vecUp + childFrame.origin.y * vecDown, 0
    );
    return topLeft;
}

Vector3 View2D::ChildLocalPosition(Rect childFrame) const {
    auto topLeft = TopLeftChildLocalPosition(childFrame);
    Vector3 result(
        topLeft.x + childFrame.size.x / 2.0f * vecRight,
        topLeft.y + (childFrame.size.y / 2.0f) * vecDown, 0
    );

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
        auto child = FirstChildView();
        GUARD(child)
        child->owner->SetLocalPosition({ 0, 0, 0 });

        GUARD(child->layout)

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
        child->SetFrame({ .origin = { frame.size.x / 2.0f - viewSize.x / 2.0f,
                                      frame.size.y / 2.0f - viewSize.y / 2.0f },
                          .size = viewSize });
        layoutRecurse(*child);
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
        value->owner = this;
    }
    layout = std::move(value);
    SetNeedsLayout();
}

VectorList<ViewProxy> View2D::ChildViewProxies() const {
    VectorList<ViewProxy> result;

    auto childViews = ChildViews();
    for (auto& child : childViews) {
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

    owner->RemoveAllChildren();
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
