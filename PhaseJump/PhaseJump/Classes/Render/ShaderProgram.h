#pragma once

#include "Base.h"
#include "StringUtils.h"
#include "UnorderedMap.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 12/21/24
 */
namespace PJ {
    namespace ShaderId {
        /// Renders a varying color
        auto constexpr ColorVary = "color.vary";

        /// Renders a single texture that is modulated by a varying color
        auto constexpr TextureVary = "texture.vary";
    } // namespace ShaderId

    /// Links a vertex and fragment shader together
    class ShaderProgram : public Base {
    public:
        String id;

        virtual ~ShaderProgram() {}

        /// Store loaded programs here for access later
        static UnorderedMap<String, SP<ShaderProgram>> registry;
    };
} // namespace PJ
