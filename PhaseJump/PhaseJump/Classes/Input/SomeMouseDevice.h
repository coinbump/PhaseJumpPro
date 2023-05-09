#ifndef PJSOMEMOUSEDEVICE_H
#define PJSOMEMOUSEDEVICE_H

#include "SomePointerDevice.h"

namespace PJ {
    class SomeMouseDevice : public SomePointerDevice {
    public:
        static SP<SomeMouseDevice> current;

        SomeMouseDevice() {
        }
    };

    // Convenience names
    using Mouse = SomeMouseDevice;
}

#endif
