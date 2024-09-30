#ifndef PJSDLCOLORBUILDER_H
#define PJSDLCOLORBUILDER_H

#include "RGBAColor.h"
#include <SDL3/SDL_render.h>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 6/13/24
 */
namespace PJ {
    struct SDLColorBuilder {
        // TODO: probably doesn't need to be in a builder
        SDL_Color Transform(RGBAColor color32) const {
            SDL_Color result;
            result.r = color32.r();
            result.g = color32.g();
            result.b = color32.b();
            result.a = color32.a();
            return result;
        }
    };
} // namespace PJ

#endif
