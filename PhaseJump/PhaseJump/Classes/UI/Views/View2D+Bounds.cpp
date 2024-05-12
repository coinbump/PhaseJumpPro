#include "View2D.h"
#include "WorldSizeAble2D.h"

using namespace PJ;
using namespace std;

void View2D::SetFrame(Rect const& value) {
    if (frame == value) { return; }
    frame = value;
    OnFrameChange();
}

Rect View2D::Bounds() const
{
    auto result = Frame();
    result.origin = Vector2::zero;
    return result;
}

void View2D::SetWorldSize2D(Vector2 value) {
    auto newFrame = Frame();
    newFrame.size = value;
    SetFrame(newFrame);
}

void View2D::OnFrameChange()
{
    UpdateFrameComponents();
    OnViewSizeChange();
}

void View2D::UpdateFrameComponents()
{
    auto frameSize = Frame().size;
    auto unscaledFrameSize = frameSize;

    // TODO: size SimplePolygonCollider2D

    // FUTURE:
//    if (TryGetComponent(out BoxCollider2D boxCollider))
//    {
//        boxCollider.offset = Vector2::zero;
//        boxCollider.size = unscaledFrameSize;
//    }
//
//    if (TryGetComponent(out CapsuleCollider2D capsuleCollider))
//    {
//        capsuleCollider.offset = Vector2::zero;
//        capsuleCollider.size = unscaledFrameSize;
//    }
//
//    if (TryGetComponent(out CircleCollider2D circleCollider))
//    {
//        circleCollider.offset = Vector2::zero;
//        circleCollider.radius = Mathf.Max(unscaledFrameSize.x, unscaledFrameSize.y) / 2.0f;
//    }

    auto worldSizables = GetComponents<WorldSizeAble2D>();
    for (auto worldSizable : worldSizables)
    {
        worldSizable->SetWorldSize2D(unscaledFrameSize);
    }
}

Rect View2D::ParentBounds()
{
    auto parent = ParentView();
    Rect parentBounds;

    // TODO:
//    if (parent)
//    {
//        parentBounds = parent.Bounds();
//    }
//    else
//    {
//        auto screenWorldSize = Utils.ScreenWorldSize(Camera);
//        // Debug.Log("ScreenWorldSize: " + screenWorldSize.ToString());
//        parentBounds.size = screenWorldSize;
//    }

    return parentBounds;
}
