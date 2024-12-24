#pragma once

#include "WorldComponent.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 12/21/24
 */
namespace PJ {
    class UIWorldSystem;

    /// Sent when an object is selected or de-selected
    class SelectEvent : public SomeSignal {
    public:
        bool isSelected{};

        SelectEvent(bool isSelected) :
            isSelected(isSelected) {}
    };

    /// Handles selection state behavior in an object canvas
    /// Default behavior: Sends signal when selection changes
    class SelectHandler : public WorldComponent<> {
    public:
        using Base = WorldComponent;
        using This = SelectHandler;
        using SelectHandlerFunc = std::function<void(This&)>;

    protected:
        bool isSelected = false;
        bool isSelectable = true;

    public:
        SelectHandlerFunc onSelectChangeFunc;

        SelectHandler();

        bool IsSelectable() const {
            return isSelectable;
        }

        bool IsSelected() const {
            return isSelected;
        }

        void SetIsSelected(bool value) {
            GUARD(isSelected != value)
            if (value && !isSelectable) {
                return;
            }

            isSelected = value;
            OnSelectChange();
        }

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "SelectHandler";
        }

    protected:
        void Awake() override {
            Base::Awake();
        }

        virtual void OnSelectChange();
    };
} // namespace PJ
