#ifndef PJSELECTHANDLER_H
#define PJSELECTHANDLER_H

#include "SomeResolver.h"
#include "WorldComponent.h"

/*
 RATING: 5 stars
 Direct port
 CODE REVIEW: 6/18/23
 */
namespace PJ {
    class UIWorldSystem;

    /// Handles selection state behavior (Example: Marquee select objects UI)
    class SelectHandler : public WorldComponent<> {
    public:
        using Base = WorldComponent;

    protected:
        bool isSelected = false;
        bool isSelectable = true;

    public:
        SP<SomeWorldComponent> selectEffect;

        SelectHandler();

        bool IsSelectable() const {
            return isSelectable;
        }

        std::function<UIWorldSystem*()> uiSystemResolver;

        virtual UIWorldSystem* UISystem() const {
            GUARDR(uiSystemResolver, nullptr)
            return uiSystemResolver();
        }

        bool IsSelected() const {
            return isSelected;
        }

        void SetIsSelected(bool value) {
            if (isSelected == value) {
                return;
            }
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

            UpdateSelectEffect();
        }

        void UpdateSelectEffect() {
            if (!selectEffect) {
                return;
            }
            selectEffect->Enable(IsSelected());
        }

        virtual void OnSelectChange();
    };
} // namespace PJ

#endif
