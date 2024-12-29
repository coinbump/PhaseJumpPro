#include "PageView.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

PageView::PageView(SP<SingleSelectStore> store) {
    SetStore(store);
}

void PageView::SetStore(SP<SingleSelectStore> value) {
    GUARD(store != value)
    store = value;

    GUARD(store)

    cancellables.clear();

    auto cancellable = store->selection.Receive([this](auto& value) { RebuildPage(); });
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
        auto& pageFunc = buildPageFuncs.at(store->selection);
        owner->DestroyAllChildren();
        pageFunc(*owner);
    } catch (...) {}
}

void PageView::SelectPage(string page) {
    GUARD(store)
    store->selection.SetValue(page);
}

void PageView::Navigate(NavigateDirection direction, bool loop) {
    GUARD(!IsEmpty(pageOrder));
    GUARD(store)

    auto selection = store->selection;
    auto optionalIndex = IndexOf(pageOrder, selection);
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
            store->selection.SetValue(pageOrder[index]);
        }
    }
}
