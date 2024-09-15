#include "View2D.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

SP<View2D> View2D::ParentView() const {
    GUARDR(owner && owner->Parent(), nullptr)

    return owner->Parent()->TypeComponent<View2D>();
}

SP<View2D> View2D::FirstChildView() const {
    GUARDR(owner, {})

    for (auto& child : owner->Children()) {
        auto view = child->TypeComponent<View2D>();
        if (view) {
            return view;
        }
    }

    return nullptr;
}

VectorList<SP<View2D>> View2D::ChildViews() const {
    GUARDR(owner, {})

    VectorList<SP<View2D>> result;

    for (auto& child : owner->Children()) {
        // Only 1 child view component supported per node
        auto view = child->TypeComponent<View2D>();
        if (view) {
            result.push_back(view);
        }
    }

    return result;
}
