#ifndef PJSOMEDEVICE_H
#define PJSOMEDEVICE_H

/*
 * RATING: 5 stars
 * Simple abstract class
 * CODE REVIEW: 11/12/22
 */
namespace PJ
{
    /// <summary>
    /// Some device that sends inputs (mouse, gamepad, etc.)
    /// </summary>
    class SomeInputDevice
    {
        public virtual bool IsAvailable() = 0;
    };
}

#endif
