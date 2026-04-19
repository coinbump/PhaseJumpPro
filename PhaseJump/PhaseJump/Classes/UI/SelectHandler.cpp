#include "SelectHandler.h"
#include "UIWorldSystem.h"

using namespace std;
using namespace PJ;

SelectHandler::SelectHandler() {
    isSelected.SetOnValueChangeFunc(SetOnValueChangeFuncType::None, [this](auto value) {
        GUARD(onIsSelectedChangeFunc)
        onIsSelectedChangeFunc(*this);
    });

    onIsSelectedChangeFunc = [this](auto& handler) {
        GUARD(owner)
        owner->Signal(SignalId::Select, SelectEvent(isSelected));
    };
}
