#ifndef PJGLSHADERLOADER_H
#define PJGLSHADERLOADER_H

#include "FilePath.h"
#include <stdio.h>

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 4/13/23
 */
namespace PJ {
    /// Manages an OpenGLShader
    class SomeGLShader;

    // TODO: make this a load operation
    class GLShaderLoader {
    public:
        bool LoadFromPath(SomeGLShader& shader, FilePath path);
    };
} // namespace PJ

#endif
