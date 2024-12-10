#include "View2D.h"
#include "WorldNode.h"
#include "WorldSizeable.h"

using namespace PJ;
using namespace std;

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
    result.origin = {};
    return result;
}

void View2D::SetWorldSize2D(Vector2 value) {
    auto newFrame = Frame();
    newFrame.size = value;
    SetFrame(newFrame);
}

void View2D::OnFrameChange() {
    SetNeedsLayout();
}

void View2D::OnViewSizeChange() {
    UpdateFrameComponents();
}

void View2D::UpdateFrameComponents() {
    GUARD(owner)

    auto frameSize = Frame().size;

    // Update all other components attached to this node to match the view size
    // Keeps meshes, layouts, etc. in sync with view
    VectorList<WorldSizeable*> worldSizables;
    owner->CollectTypeComponents<WorldSizeable>(worldSizables, [this](auto& component) {
        return &component != this;
    });
    for (auto& worldSizable : worldSizables) {
        worldSizable->SetWorldSize(frameSize);
    }
}

std::optional<Rect> View2D::ParentBounds() {
    auto parent = ParentView();
    GUARDR(parent, {})

    return parent->Bounds();
}
