#include "GLShaderLoader.h"
#include "GLHeaders.h"
#include "SomeGLShader.h"
#include <fstream>

using namespace std;
using namespace PJ;

#define _VERBOSE_PHASE_

bool GLShaderLoader::LoadFromPath(SomeGLShader& shader, FilePath path)
{
    if (shader.isCompiled) { return false; }    // Shader already loaded

    if (0 == shader.glType) {
        PJLog("ERROR. No shader type defined.");
        return 0;
    }

    GLint status = 0;
    const GLchar *shaderChars;

    // Load the shader from the file
    String shaderString;
    std::ifstream file;
    file.open(path);
    if (file.is_open()) {
        std::stringstream buffer;

    #ifndef _OPENGLES_PHASE_
        // OpenGL 3+ requires version as first line. Mac OS defaults to 150
        buffer << "#version 150\n";
        buffer << "#define _PREMULT_PHASE_\n";
        
        // OpenGL ES requires highp, mediump, lowp qualifiers for fragment shaders,
        // But these are undefined in standard OpenGL, so we define them here.
        // If we don't, the shader won't compile on desktop
        // OBSOLETE: New shaders can't be both OpenGL and OpenGL ES compatible
        // buffer << "#define highp\n#define mediump\n#define lowp\n";
    #endif

        buffer << file.rdbuf();
        shaderString = buffer.str();
    } else {
        PJLog("ERROR. Can't open shader file at: %s", path.c_str());
        return false;
    }

    shaderChars = (GLchar*)shaderString.c_str();

    auto glId = glCreateShader(shader.glType);  // Create shader
    glShaderSource(glId, 1, &shaderChars, NULL);    // Set source code in the shader
    glCompileShader(glId);  // Compile shader

    // Output a status log.
    GLint logLength;
    glGetShaderiv(glId, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar*)malloc(logLength);
        glGetShaderInfoLog(glId, logLength, &logLength, log);
        PJLog("Shader compile log:\n%s", log);
        free(log);
    }

    // Check to see if the shader compiled.
    glGetShaderiv(glId, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        PJLog("Failed to compile shader:\n");
    }
    else {
        shader.glId = glId;
        shader.isCompiled = true;
    }

    return shader.isCompiled;
}
