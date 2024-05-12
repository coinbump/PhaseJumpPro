#ifndef PJWORLDSIZEABLE2D_H
#define PJWORLDSIZEABLE2D_H

#include "Vector2.h"

namespace PJ
{
    class WorldSizeAble2D {
    public:
        virtual ~WorldSizeAble2D() {}
        
        virtual Vector2 WorldSize2D() const = 0;
        virtual void SetWorldSize2D(Vector2 value) = 0;
    };
}

#endif
