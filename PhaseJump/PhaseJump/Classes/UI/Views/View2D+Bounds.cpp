#include "View2D.h"
#include "WorldSizeable.h"

using namespace PJ;
using namespace std;

/*
 RATING: 5 stars
 Simple funcs
 CODE REVIEW: 9/7/24
 */
View2D& View2D::SetFrame(Rect const& value) {
    GUARDR(frame != value, *this)
    bool didFrameSizeChange = frame.size != value.size;
    frame = value;
    OnFrameChange();

    if (didFrameSizeChange) {
        OnViewSizeChange();
    }

    return *this;
}

Rect View2D::Bounds() const {
    auto result = Frame();
    result.origin = vec2Zero;
    return result;
}

void View2D::Awake() {
    UpdateFrameComponents();
}

void View2D::SetWorldSize2D(Vector2 value) {
    auto newFrame = Frame();
    newFrame.size = value;
    SetFrame(newFrame);
}

void View2D::OnFrameChange() {}

void View2D::OnViewSizeChange() {
    UpdateFrameComponents();
}

void View2D::UpdateFrameComponents() {
    auto frameSize = Frame().size;

    auto worldSizables = GetComponents<WorldSizeable>();
    for (auto& worldSizable : worldSizables) {
        worldSizable->SetWorldSize(frameSize);
    }
}

std::optional<Rect> View2D::ParentBounds() {
    auto parent = ParentView();
    GUARDR(parent, std::nullopt)

    return parent->Bounds();
}
