#include "ToggleButtonControl.h"

using namespace std;
using namespace PJ;

ToggleButtonControl::ToggleButtonControl(Binding<bool> _isOnBinding) :
    isOnBinding(_isOnBinding) {
    isOn.SetOnValueChangeFunc(SetOnValueChangeFuncType::Sync, [this](auto value) {
        isOnBinding.SetValue(value);
        OnViewStateChange();
    });

    tracker.onPressFunc = [this]() { OnPress(); };
    tracker.onStateChangeFunc = [this]() { OnViewStateChange(); };

    AddSignalHandler<PointerDownUIEvent>({ .id = SignalId::PointerDown,
                                           .func = [this](auto& component, auto& event) {
                                               tracker.OnPointerDown();
                                           } });

    AddSignalHandler<PointerEnterUIEvent>({ .id = SignalId::PointerEnter,
                                            .func = [this](auto& component, auto& event) {
                                                tracker.OnPointerEnter();
                                            } });

    AddSignalHandler<PointerExitUIEvent>({ .id = SignalId::PointerExit,
                                           .func = [this](auto& component, auto& event) {
                                               tracker.OnPointerExit();
                                           } });

    AddSignalHandler<PointerUpUIEvent>({ .id = SignalId::PointerUp,
                                         .func = [this](auto& component, auto& event) {
                                             tracker.OnPointerUp();
                                         } });

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
    SetIsToggleOn(!IsToggleOn());
}
