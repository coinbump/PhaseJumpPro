#ifndef PJCAMERA_H
#define PJCAMERA_H

#include <memory>

namespace PJ {
    class SomeCamera;

    /// Static convenience methods for camera access (managed by World)
    class Camera {
    public:
        static SP<SomeCamera> main;
    };
}

#endif
