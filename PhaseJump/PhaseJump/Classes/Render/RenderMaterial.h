#ifndef PJMATERIAL_H
#define PJMATERIAL_H

#include "Tags.h"
#include "Color.h"
#include "VectorList.h"
#include "_Set.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 4/13/23
 */
namespace PJ {
    class SomeShaderProgram;

    struct RenderTexture {
        uint32_t id = 0;

        RenderTexture(uint32_t id) : id(id) {
        }
    };

    /// Combies render properties for object
    class RenderMaterial {
    public:
        std::shared_ptr<SomeShaderProgram> shaderProgram;
        VectorList<Color> uniformColors;
        VectorList<Color> colors;
        VectorList<RenderTexture> textures;
        Set<String> features;
    };
}

#endif
