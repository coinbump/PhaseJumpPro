#ifndef PJSOMETEXTURE_H
#define PJSOMETEXTURE_H

#include "Vector2.h"

/*
 RATING: 5 stars
 Simple interface
 CODE REVIEW: 12/24/22
 */
namespace PJ {
    /// <summary>
    /// A GPU texture that can be rendered in a Render Context
    /// </summary>
    class SomeTexture {
    public:
        Vector2Int size;

        SomeTexture(Vector2Int size) : size(size) {
        }

        virtual ~SomeTexture() { }
    };
}

#endif
