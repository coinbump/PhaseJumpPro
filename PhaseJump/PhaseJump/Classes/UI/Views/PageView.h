#pragma once

#include "PublishedValue.h"
#include "View2D.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/1/24
 */
namespace PJ {
    /// Stores model state for page view
    class PageViewStore {
    public:
        PublishedValue<String> selectedPage;

        bool HasSelection() {
            return !IsEmpty(selectedPage.Value());
        }
    };

    /// Shows a single page based on user selection
    class PageView : public View2D {
    public:
        using Base = View2D;
        using This = PageView;

        using BuildPageFunc = std::function<void(WorldNode&)>;
        using BuildPageFuncsMap = UnorderedMap<String, BuildPageFunc>;

    protected:
        /// Shared state
        SP<PageViewStore> store;

        /// Stores subscriptions to reactive values
        UnorderedSet<SP<Cancellable>> cancellables;

    public:
        enum class NavigateDirection { Forward, Back };

        struct PageConfig {
            String id;
            BuildPageFunc buildPageFunc;
        };

        PageViewStore* Store() const {
            return store.get();
        }

        void SetStore(SP<PageViewStore> value);

        /// View builder funcs for each page
        BuildPageFuncsMap buildPageFuncs;

        /// Order of pages by id
        VectorList<String> pageOrder;

        PageView(SP<PageViewStore> store);

        void SelectPage(String page);
        void Navigate(NavigateDirection direction, bool loop);
        void RebuildPage();

        // MARK: View2D

        void Rebuild() override;
    };
} // namespace PJ
