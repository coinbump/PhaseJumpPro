#ifndef PJRENDERFEATURES_H
#define PJRENDERFEATURES_H

#include "_String.h"

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: 4/14/23
 */
namespace PJ {
    namespace RenderFeatures {
        /*
         Enables transparent blending.

         Blend has a performance cost and is usually avoided for 3D renders
         For 2D renders, render unblended objects first, then sort transparent-blend objects back-to-front
         */
        static const String Blend = "blend";

        /*
         Enables optional clipping of rendered output
         */
        static const String ScissorTest = "scissorTest";
    };
}

#endif
