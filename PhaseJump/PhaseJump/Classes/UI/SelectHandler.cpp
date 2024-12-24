#include "SelectHandler.h"
#include "UIWorldSystem.h"

using namespace std;
using namespace PJ;

SelectHandler::SelectHandler() {
    onSelectChangeFunc = [this](auto& handler) {
        GUARD(owner)
        owner->Signal("selected", SelectEvent(isSelected));
    };
}

void SelectHandler::OnSelectChange() {
    GUARD(onSelectChangeFunc);
    onSelectChangeFunc(*this);
}
