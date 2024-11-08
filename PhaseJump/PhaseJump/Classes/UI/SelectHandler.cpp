#include "SelectHandler.h"
#include "UIWorldSystem.h"

using namespace std;
using namespace PJ;

SelectHandler::SelectHandler() {
    uiSystemResolver = []() { return UIWorldSystem::shared; };
}

void SelectHandler::OnSelectChange() {
    UpdateSelectEffect();

    auto uiSystem = UISystem();
    if (uiSystem) {
        uiSystem->UpdateSelectionFor(SCAST<SelectHandler>(shared_from_this()));
    }
}
