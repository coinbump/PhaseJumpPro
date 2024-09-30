#include "UIControl2D.h"

using namespace std;
using namespace PJ;

void UIControl2D::Awake() {
    Base::Awake();

    OnStateChange();
}

void UIControl2D::OnStateChange() {
    GUARD(onControlUpdateFunc)
    onControlUpdateFunc(*this);
}
