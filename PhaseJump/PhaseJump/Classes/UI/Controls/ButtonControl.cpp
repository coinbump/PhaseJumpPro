#include "ButtonControl.h"
#include "Utils.h"

using namespace std;
using namespace PJ;

ButtonControl::ButtonControl() {
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
}

void ButtonControl::Awake() {
    Base::Awake();
}

void ButtonControl::OnPress() {
    GUARD(onPressFunc)
    onPressFunc(*this);
}
