#pragma once

#include "SomePointerDevice.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 8/11/24
 */
namespace PJ {
    class SomeMouseDevice : public SomePointerDevice {
    public:
        SomeMouseDevice() {}
    };

    // MARL: Convenience

    using Mouse = SomeMouseDevice;

} // namespace PJ
