#ifndef PJGLSHADERPROGRAMLOADER_H
#define PJGLSHADERPROGRAMLOADER_H

#include "FilePath.h"
#include "GLShaderProgram.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 4/14/23
 */
namespace PJ
{
    class GLShaderProgram;
    
    class GLShaderProgramLoader {
    public:
        /// Load from a .shprog JSON file that specifies which shaders to use
        std::optional<GLShaderProgram::Info> InfoFromPath(FilePath path);
        SP<GLShaderProgram> LoadFromShaderPaths(FilePath vertexShaderPath, FilePath fragmentShaderPath);
    };
}

#endif
