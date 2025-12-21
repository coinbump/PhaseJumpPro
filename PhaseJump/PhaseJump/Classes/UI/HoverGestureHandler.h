#pragma once

#include "SomeUIEvent.h"
#include "Valve.h"
#include "WorldComponent.h"

/*
 RATING: 5 stars
 Simple protocol
 Code review: 9/22/24
 */
namespace PJ {
    /// Sent when an object's hover state changes
    class HoverUIEvent : public SomeSignal {
    public:
        bool isHovering{};

        HoverUIEvent(bool isHovering) :
            isHovering(isHovering) {}
    };

    /// Receives hover gesture signals
    /// The UI system controls which node has hover state
    /// Only 1 object can have hover state at a time
    class HoverGestureHandler : public WorldComponent {
    protected:
        bool isHovering{};

    public:
        using Base = WorldComponent;
        using This = HoverGestureHandler;

        HoverGestureHandler();

        virtual ~HoverGestureHandler() {}

        bool IsHovering() const {
            return isHovering;
        }

        virtual void SetIsHovering(bool value) {
            isHovering = value;
        }

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "HoverGestureHandler";
        }
    };

    /// Turns valve on/off based on hover state
    class ValveHoverGestureHandler : public HoverGestureHandler {
    public:
        using Base = HoverGestureHandler;
        using This = ValveHoverGestureHandler;

        // SP so we can add to updatables
        Valve valve;

        float turnOnDuration = 0.3f;
        float turnOffDuration = 0.3f;

        ValveHoverGestureHandler(float turnOnDuration = 0.3f, float turnOffDuration = 0.3f);

        // MARK: HoverGestureHandler

        void SetIsHovering(bool value) override;

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "ValveHoverGestureHandler";
        }
    };
} // namespace PJ
