#pragma once

#include "StringUtils.h"

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: 1/1/25
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

        /// Enables optional clipping of rendered output
        auto constexpr ScissorTest = "scissorTest";

        /// Enables culling of back faces
        auto constexpr CullFace = "cullFace";

        /// Enables 2D textures
        auto constexpr Texture2D = "texture2D";

        /// Enables Z-ordering via depth testing
        auto constexpr DepthTest = "depthTest";

        /// Enables MSAA rendering
        auto constexpr MultiSample = "multiSample";

    }; // namespace RenderFeature
} // namespace PJ
