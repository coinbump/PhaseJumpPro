#ifndef PJSOMEDEVICE_H
#define PJSOMEDEVICE_H

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 7/13/24
 */
namespace PJ {
    /// Some device that sends inputs (mouse, gamepad, etc.)
    class SomeInputDevice {
    public:
        virtual ~SomeInputDevice() {}

        virtual bool IsAvailable() = 0;
    };
} // namespace PJ

#endif
