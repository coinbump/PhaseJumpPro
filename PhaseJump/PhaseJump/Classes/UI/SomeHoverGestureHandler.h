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
    class HoverUIEvent : public SomeUIEvent {
    public:
        bool isHovering{};

        HoverUIEvent(bool isHovering) :
            isHovering(isHovering) {}
    };

    /// Receives hover gesture signals
    /// The UI system controls which node has hover state
    /// Only 1 object can have hover state at a time
    class SomeHoverGestureHandler : public WorldComponent<> {
    protected:
        bool isHovering{};

    public:
        using Base = WorldComponent<>;
        using This = SomeHoverGestureHandler;

        SomeHoverGestureHandler();

        virtual ~SomeHoverGestureHandler() {}

        bool IsHovering() const {
            return isHovering;
        }

        virtual void SetIsHovering(bool value) {
            isHovering = value;
        }
    };

    /// Turns valve on/off based on hover state
    class ValveHoverGestureHandler : public SomeHoverGestureHandler {
    public:
        using Base = SomeHoverGestureHandler;
        using This = ValveHoverGestureHandler;

        // SP so we can add to updatables
        SP<Valve> valve = MAKE<Valve>();

        float turnOnDuration = 0.3f;
        float turnOffDuration = 0.3f;

        ValveHoverGestureHandler(float turnOnDuration = 0.3f, float turnOffDuration = 0.3f);

        // MARK: SomeHoverGestureHandler

        void SetIsHovering(bool value) override;

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "HoverGestureHandler";
        }
    };
} // namespace PJ
