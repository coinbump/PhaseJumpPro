#include "ToggleButtonControl.h"

using namespace std;
using namespace PJ;

ToggleButtonControl::ToggleButtonControl(Binding<bool> _isOnBinding) :
    isOnBinding(_isOnBinding) {
    isOn.SetOnValueChangeFunc([this](auto value) {
        isOnBinding.SetValue(value);
        OnControlChange();
    });

    GetUpdatable().onUpdateFunc = [this](auto& updatable, auto time) {
        if (isOnBinding.getFunc) {
            SetIsToggleOn(isOnBinding.getFunc());
        }

        return FinishType::Continue;
    };
}

void ToggleButtonControl::SetIsOnBinding(Binding<bool> value) {
    isOnBinding = value;
    if (isOnBinding.getFunc) {
        SetIsToggleOn(isOnBinding.getFunc());
    }
}

bool ToggleButtonControl::IsToggleOn() const {
    return isOn;
}

void ToggleButtonControl::SetIsToggleOn(bool value) {
    isOn.SetValue(value);
}

void ToggleButtonControl::OnPress() {
    Base::OnPress();

    SetIsToggleOn(!IsToggleOn());
}
