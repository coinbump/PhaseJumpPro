#ifndef PJSDLCOLORBUILDER_H
#define PJSDLCOLORBUILDER_H

#include <SDL2/SDL_render.h>

/*
 RATING: 5 stars
 Simple builder
 CODE REVIEW: 12/27/22
 */
namespace PJ {
    struct SDLColorBuilder
    {
        SDL_Color Transform(Color32 color32) const
        {
            SDL_Color result;
            result.r = color32.r();
            result.g = color32.g();
            result.b = color32.b();
            result.a = color32.a();
            return result;
        }
    };
}

#endif
