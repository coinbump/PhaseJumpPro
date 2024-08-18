#pragma once

#include "Base.h"
#include "Class.h"
#include "Vector2.h"

/*
 RATING: 4 stars
 Simple protocol, no functionality (yet)
 CODE REVIEW: 8/8/24
 */
namespace PJ {
    class SomePlatformWindow : public Base {
    public:
        virtual Vector2Int Size() const = 0;
        virtual Vector2Int PixelSize() const = 0;
    };
} // namespace PJ
