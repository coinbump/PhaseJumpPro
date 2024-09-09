#include "View2D.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

View2D* View2D::ParentView() const {
    GUARDR(owner, nullptr)
    return owner->TypeComponent<View2D>().get();
}

SP<View2D> View2D::FirstChildView() const {
    // TODO:
    return nullptr;
}
