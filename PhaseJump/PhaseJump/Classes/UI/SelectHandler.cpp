#include "SelectHandler.h"
#include "UISystem.h"
#include "UISystemSharedDependencyResolver.h"

using namespace std;
using namespace PJ;

SelectHandler::SelectHandler() : uiSystemDependencyResolver(MAKE<UISystemSharedDependencyResolver>()) {
}

void SelectHandler::OnSelectChange()
{
    UpdateSelectEffect();

    auto uiSystem = UISystem();
    if (uiSystem)
    {
        uiSystem->UpdateSelectionFor(SCAST<SelectHandler>(shared_from_this()));
    }
}
