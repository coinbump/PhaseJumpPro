#ifndef PJSELECTHANDLER_H
#define PJSELECTHANDLER_H

#include "SomeEffect.h"
#include "SomeResolver.h"
#include "WorldComponent.h"

/*
 RATING: 5 stars
 Direct port
 CODE REVIEW: 6/18/23
 */
namespace PJ {
    class UISystem;

    /// Handles selection state behavior (Example: Marquee select objects UI)
    class SelectHandler : public WorldComponent<> {
    public:
        using Base = WorldComponent;

    protected:
        bool isSelected = false;
        bool isSelectable = true;

    public:
        SP<SomeEffect> selectEffect;

        SelectHandler();

        bool IsSelectable() const {
            return isSelectable;
        }

        std::function<UISystem*()> uiSystemResolver;

        virtual UISystem* UISystem() const {
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

    protected:
        void Awake() override {
            Base::Awake();

            UpdateSelectEffect();
        }

        void UpdateSelectEffect() {
            if (!selectEffect) {
                return;
            }
            selectEffect->SetIsOn(IsSelected());
        }

        virtual void OnSelectChange();
    };
} // namespace PJ

#endif
