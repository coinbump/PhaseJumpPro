#include "UIControl2D.h"

using namespace std;
using namespace PJ;

void UIControl2D::Awake() {
    Base::Awake();

    OnStateChange();

    GUARD(owner)
    owner->AddComponent<HoverGestureHandler>(SCAST<UIControl2D>(shared_from_this()));
}

void UIControl2D::OnStateChange() {
    GUARD(onControlUpdateFunc)
    onControlUpdateFunc(*this);
}
