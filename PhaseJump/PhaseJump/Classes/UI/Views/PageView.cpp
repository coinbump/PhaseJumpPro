#include "PageView.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

PageView::PageView(SP<PageViewStore> store) {
    SetStore(store);
}

void PageView::SetStore(SP<PageViewStore> value) {
    GUARD(store != value)
    store = value;

    GUARD(store)

    cancellables.clear();

    auto cancellable = store->selectedPage.Receive([this](auto& value) { RebuildPage(); });
    cancellables.insert(cancellable);
}

void PageView::Rebuild() {
    Base::Rebuild();
    RebuildPage();
}

void PageView::RebuildPage() {
    GUARD(store)
    GUARD(store->HasSelection())
    GUARD(owner);

    try {
        auto& pageFunc = buildPageFuncs.at(store->selectedPage);
        owner->RemoveAllChildren();
        pageFunc(*owner);
    } catch (...) {}
}

void PageView::SelectPage(string page) {
    GUARD(store)
    store->selectedPage.SetValue(page);
}

void PageView::Navigate(NavigateDirection direction, bool loop) {
    GUARD(!IsEmpty(pageOrder));
    GUARD(store)

    auto selectedPage = store->selectedPage;
    auto optionalIndex = IndexOf(pageOrder, selectedPage);
    int index{};

    if (optionalIndex) {
        index = (int)*optionalIndex;

        switch (direction) {
        case NavigateDirection::Forward:
            index++;
            if (loop && index >= pageOrder.size()) {
                index = 0;
            }
            break;
        case NavigateDirection::Back:
            index--;
            if (loop && index < 0) {
                index = (int)pageOrder.size() - 1;
            }
            break;
        }

        if (index >= 0 && index < pageOrder.size()) {
            store->selectedPage.SetValue(pageOrder[index]);
        }
    }
}
