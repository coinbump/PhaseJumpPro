#include "SelectHandler.h"
#include "UIWorldSystem.h"

using namespace std;
using namespace PJ;

void UIWorldSystem::SetSelection(OrderedSet<SP<SelectHandler>> const& value) {
    auto removedSelectHandlers = selection - value;

    for (auto& selectHandler : removedSelectHandlers) {
        selectHandler->SetIsSelected(false);
    }

    for (auto& selectHandler : value) {
        selectHandler->SetIsSelected(true);
    }

    selection = value;
}

void UIWorldSystem::UpdateSelectionFor(SP<SelectHandler> selectHandler) {
    if (selectHandler->IsSelected()) {
        selection.insert(selectHandler);
    } else {
        selection.erase(selectHandler);
    }
}
