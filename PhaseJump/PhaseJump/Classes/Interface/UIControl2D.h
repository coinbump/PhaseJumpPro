#ifndef PJUICONTROL2D_H
#define PJUICONTROL2D_H

#include "View2D.h"
#include "SomeEventHandler.h"
#include "SomeHoverGestureHandler.h"

/*
 RATING: 4 stars
 Tested and works. Needs unit tests
 CODE REVIEW: 5/8/23
 */
namespace PJ
{
    /// <summary>
    /// An interactive UI object that accepts inputs from the user and produces events.
    /// Example: button, switch, checkbox, etc.
    /// </summary>
    class UIControl2D : public View2D
    {
    protected:
        /// <summary>
        /// Component that handles events (Example: Button.PressEvent)
        /// </summary>
        SP<SomeEventHandler> eventHandler;

        class HoverGestureHandler : public SomeHoverGestureHandler
        {
        public:
            WP<UIControl2D> target;

            void SetIsHovering(bool value) override
            {
                if (target.expired()) { return; }
                target.lock()->isHovering = value;
            }
        };
    
    protected:
        SP<HoverGestureHandler> hoverGestureHandler;

        bool isHovering = false;

    public:
        using Base = View2D;

        virtual void SetIsHovering(bool value) {
            isHovering = value;
        }

        SP<SomeListener> Listener() const {
            auto result = eventHandler;
            if (nullptr == result) {
                // Dispatch events to UISystem if no event handler is connected
                // Allows us to centralize UI logic if needed
                // FUTURE: result = UISystem::shared;
            }
            return result;
        }

    protected:
        void Awake() override;
    };
}

#endif
