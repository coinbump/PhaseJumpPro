#ifndef PJMATERIAL_H
#define PJMATERIAL_H

#include "Tags.h"
#include "Color.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 4/13/23
 */
namespace PJ {
    class SomeShaderProgram;

    struct RenderTexture {
        uint32_t id = 0;
    };

    /// Combies render properties for object
    class RenderMaterial {
    public:
        std::shared_ptr<SomeShaderProgram> shaderProgram;
        VectorList<Color> uniformColors;
        VectorList<RenderTexture> textures;

        // FUTURE: support custom parameter ordering for display if needed
        /// Shader parameters
        std::shared_ptr<Tags> tags = std::make_shared<Tags>();
    };
}

#endif
