#ifndef PJSELECTHANDLER_H
#define PJSELECTHANDLER_H

#include "WorldComponent.h"
#include "SomeEffect.h"
#include "SomeDependencyResolver.h"

/*
 RATING: 5 stars
 Direct port
 CODE REVIEW: 6/18/23
 */
namespace PJ
{
    class UISystem;

    /// <summary>
    /// Handles selection state behavior (Example: Marquee select objects UI)
    /// </summary>
    class SelectHandler : public WorldComponent
    {
    public:
        using Base = WorldComponent;

    protected:
        bool isSelected = false;
        bool isSelectable = true;

    public:
        SP<SomeEffect> selectEffect;

        SelectHandler();
        
        bool IsSelectable() const { return isSelectable; }

        SP<SomeDependencyResolver<SP<UISystem>>> uiSystemDependencyResolver;

        virtual SP<UISystem> UISystem() const
        {
            return uiSystemDependencyResolver->Dependency();
        }

        bool IsSelected() const { return isSelected; }
        void SetIsSelected(bool value)
        {
            if (isSelected == value)
            {
                return;
            }
            if (value && !isSelectable)
            {
                return;
            }

            isSelected = value;
            OnSelectChange();
        }

    protected:
        void Awake() override
        {
            Base::Awake();

            UpdateSelectEffect();
        }

        void UpdateSelectEffect()
        {
            if (!selectEffect) { return; }
            selectEffect->SetIsOn(IsSelected());
        }

        virtual void OnSelectChange();
    };
}

#endif
