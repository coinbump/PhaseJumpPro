#ifndef PJCAMERA_H
#define PJCAMERA_H

#include "Utils.h"
#include <memory>

// CODE REVIEW: ?/23
namespace PJ {
    class SomeCamera;

    /// Static convenience methods for camera access (managed by World)
    class Camera {
    public:
        static SP<SomeCamera> main;

        virtual ~Camera() {}
    };
} // namespace PJ

#endif
