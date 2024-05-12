#include "UIControl2D.h"

using namespace std;
using namespace PJ;

void UIControl2D::Awake() {
    Base::Awake();

    auto node = Node();
    if (!node) { return; }

    hoverGestureHandler = node->AddComponent<HoverGestureHandler>();
    hoverGestureHandler->target = SCAST<UIControl2D>(shared_from_this());
}
