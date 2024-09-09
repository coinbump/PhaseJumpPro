#pragma once

#include "StringUtils.h"

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: 8/8/24
 */
namespace PJ {
    namespace RenderFeature {
        /*
         Enables transparent blending.

         Blend has a performance cost and is usually avoided for 3D renders
         For 2D renders, render unblended objects first, then sort
         transparent-blend objects back-to-front
         */
        auto constexpr Blend = "blend";

        /*
         Enables optional clipping of rendered output
         */
        auto constexpr ScissorTest = "scissorTest";
    }; // namespace RenderFeature
} // namespace PJ
