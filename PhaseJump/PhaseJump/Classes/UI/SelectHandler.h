#pragma once

#include "ObservedValue.h"
#include "WorldComponent.h"

/*
 RATING: 5+ stars
 Has unit tests
 CODE REVIEW: 4/18/26
 */
namespace PJ {
    class UIWorldSystem;

    /// Sent when an object is selected or de-selected
    class SelectEvent final : public SomeSignal {
    public:
        bool isSelected{};

        SelectEvent(bool isSelected) :
            isSelected(isSelected) {}
    };

    /// Handles selection state behavior in an object canvas
    /// Default behavior: Sends signal when selection changes
    class SelectHandler : public WorldComponent {
    public:
        using Base = WorldComponent;
        using This = SelectHandler;
        using OnIsSelectedChangeFunc = std::function<void(This&)>;

    protected:
        ObservedValue<bool> isSelected{ false };
        bool isSelectable = true;

    public:
        /// Default behavior is to send a signal to the owner, or you can replace this
        OnIsSelectedChangeFunc onIsSelectedChangeFunc;

        SelectHandler();

        bool IsSelectable() const {
            return isSelectable;
        }

        void SetIsSelectable(bool value) {
            isSelectable = value;
        }

        bool IsSelected() const {
            return isSelected;
        }

        void SetIsSelected(bool value) {
            if (value && !isSelectable) {
                return;
            }
            isSelected = value;
        }

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "SelectHandler";
        }
    };
} // namespace PJ
