#ifndef PJSOMEDEVICE_H
#define PJSOMEDEVICE_H

/*
 RATING: 5 stars
 Simple class
 CODE REVIEW: 5/8/23
 */
namespace PJ
{
    /// <summary>
    /// Some device that sends inputs (mouse, gamepad, etc.)
    /// </summary>
    class SomeInputDevice
    {
    public:
        virtual ~SomeInputDevice() {}
        
        virtual bool IsAvailable() = 0;
    };
}

#endif
