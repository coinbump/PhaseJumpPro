#ifndef PJMATERIAL_H
#define PJMATERIAL_H

#include "Tags.h"
#include "Color.h"
#include "VectorList.h"
#include "_Set.h"
#include "_Map.h"
#include "RenderTypes.h"
#include "SomeTexture.h"
#include "Vector2.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 4/13/23
 */
namespace PJ {
    class SomeShaderProgram;
    
    /// Combies render properties for object
    class RenderMaterial {
    public:
        SP<SomeShaderProgram> shaderProgram;

        VectorList<Color> uniformColors;
        VectorList<float> uniformFloats;

        VectorList<Color> colors;
        VectorList<SP<SomeTexture>> textures;
        Map<String, RenderFeatureStatus> features;
    };
}

#endif
