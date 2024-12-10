#pragma once

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 12/1/24
 */
namespace PJ {
    /// Some device that sends inputs (mouse, gamepad, etc.)
    class SomeInputDevice {
    public:
        virtual ~SomeInputDevice() {}

        virtual bool IsAvailable() = 0;
    };
} // namespace PJ
