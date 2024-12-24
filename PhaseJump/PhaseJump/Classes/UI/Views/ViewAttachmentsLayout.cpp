#include "ViewAttachmentsLayout.h"
#include "View2D.h"

using namespace std;
using namespace PJ;

void ViewAttachmentsLayout::CalculateFrames(
    Vector2 boundsSize, VectorList<ViewProxy> const& children
) {
    calculatedFrames.clear();

    auto reference = FirstIf(children, [](auto& child) { return !child.view->isAttachment; });

    if (!reference) {
        Base::CalculateFrames(boundsSize, children);
        return;
    }

    // Calculate reference as if it is the only view in the ZStack
    // and use its size to size the attachments
    Base::CalculateFrames(boundsSize, { *reference });
    GUARD(!IsEmpty(calculatedFrames))

    auto frame = calculatedFrames[0];
    calculatedFrames.clear();

    // All child frames match the size of the match view
    Repeat(children.size(), [&]() { calculatedFrames.push_back(frame); });
}
