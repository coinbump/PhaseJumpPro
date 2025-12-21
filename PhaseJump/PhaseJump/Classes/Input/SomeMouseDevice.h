#pragma once

#include "SomePointerDevice.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 12/19/25
 */
namespace PJ {
    class SomeMouseDevice : public SomePointerDevice {
    public:
        SomeMouseDevice() {}
    };

    extern UP<SomeMouseDevice> currentMouseDevice;

} // namespace PJ
