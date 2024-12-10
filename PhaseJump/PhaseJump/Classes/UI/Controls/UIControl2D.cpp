#include "UIControl2D.h"

using namespace std;
using namespace PJ;

void UIControl2D::Awake() {
    Base::Awake();

    OnControlChange();
}

void UIControl2D::OnControlChange() {
    GUARD(onControlChangeFunc)
    onControlChangeFunc(*this);
}
