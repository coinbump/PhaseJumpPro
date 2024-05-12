#include "UISystem.h"
#include "SelectHandler.h"

using namespace std;
using namespace PJ;

void UISystem::SetSelection(Set<SP<SelectHandler>> const& value)
{
    auto removedSelectHandlers = selection - value;

    for (auto selectHandler : removedSelectHandlers)
    {
        selectHandler->SetIsSelected(false);
    }

    for (auto selectHandler : value)
    {
        selectHandler->SetIsSelected(true);
    }

    selection = value;
}

void UISystem::UpdateSelectionFor(SP<SelectHandler> selectHandler)
{
    if (selectHandler->IsSelected())
    {
        selection.Add(selectHandler);
    }
    else
    {
        selection.Remove(selectHandler);
    }
}
