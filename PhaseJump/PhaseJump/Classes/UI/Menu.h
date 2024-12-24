#pragma once

#include "StringUtils.h"
#include "Tags.h"
#include "UnorderedSet.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/21/24
 */
namespace PJ {
    using EventModifierType = String;

    /// Defines a keyboard shortcut with a key command and modifiers
    struct KeyboardShortcut {
        using ModifierSet = UnorderedSet<EventModifierType>;

        String value;
        ModifierSet modifiers;

        /// @return Returns a user-displayable string for the shortcut
        String ToString() const;
    };

    /// Something inside a menu
    class SomeMenuItem {
    public:
        virtual ~SomeMenuItem() {}
    };

    class SeparatorMenuItem : public SomeMenuItem {};

    /// Standard menu item
    class MenuItem : public SomeMenuItem {
    public:
        using Func = std::function<void(MenuItem const&)>;
        using BoolFunc = std::function<bool(MenuItem const&)>;
        using StringFunc = std::function<String(MenuItem const&)>;

        /// Allows menu items to be updated dynamically
        BoolFunc isToggleOnFunc;
        BoolFunc isEnabledFunc;
        StringFunc titleFunc;

        VectorList<KeyboardShortcut> shortcuts;

    protected:
        String title;

        /// List of secondary menu items
        VectorList<MenuItem> items;

        /// If true, menu item has a toggle checkmark
        bool isToggleOn{};

        /// If true, menu item is enabled
        bool isEnabled = true;

    public:
        String Title() const {
            GUARDR(titleFunc, title)
            return titleFunc(*this);
        }

        bool IsToggleOn() const {
            GUARDR(isToggleOnFunc, isToggleOn)
            return isToggleOnFunc(*this);
        }

        bool IsEnabled() const {
            GUARDR(isEnabledFunc, isEnabled)
            return isEnabledFunc(*this);
        }

        /// Called when the menu item is selected
        Func func;

        /// (Optional). Custom properties
        Tags tags;

        MenuItem(String title, VectorList<KeyboardShortcut> const& shortcuts, Func func) :
            title(title),
            shortcuts(shortcuts),
            func(func) {}

        void Run() {
            GUARD(func)
            func(*this);
        }
    };

    /// Contains menu items for user interaction
    class Menu {
    public:
        using This = Menu;
        using MenuItemList = VectorList<UP<SomeMenuItem>>;

        /// User displayed title
        String title;

        /// List of menu items
        MenuItemList items;

        /// Custom properties
        Tags tags;

        Menu(String title) :
            title(title) {}

        // Is this needed or does the menu builder handle key events?
        // FUTURE: void OnKeyDown(KeyDownUIEvent const& event)
    };
} // namespace PJ
