#include "GLLoadShaderOperation.h"
#include "Dev.h"
#include "GLHeaders.h"
#include "SomeGLShader.h"
#include <fstream>

using namespace std;
using namespace PJ;

SomeLoadResourcesOperation::Result GLLoadShaderOperation::LoadResources() {
    GUARDR(factoryFunc, Failure())

    SP<SomeGLShader> shader = factoryFunc();
    GUARDR(shader, Failure())

    GLint status = 0;
    const GLchar* shaderChars{};

    // Load the shader from the file
    String shaderString;
    std::ifstream file;
    file.open(info.filePath);

    if (file.is_open()) {
        std::stringstream buffer;

#ifndef _OPENGLES_PHASE_
        // OpenGL 3+ requires version as first line. Mac OS defaults to 150
        buffer << "#version 150\n";
        buffer << "#define _PREMULT_PHASE_ 1\n";

        // OpenGL ES requires highp, mediump, lowp qualifiers for fragment
        // shaders, But these are undefined in standard OpenGL, so we define
        // them here. If we don't, the shader won't compile on desktop OBSOLETE:
        // New shaders can't be both OpenGL and OpenGL ES compatible buffer <<
        // "#define highp\n#define mediump\n#define lowp\n";
#endif

        buffer << file.rdbuf();
        shaderString = buffer.str();
    } else {
        PJ::Log("ERROR. Can't open shader file at: ", info.filePath);
        return Failure();
    }

    shaderChars = (GLchar*)shaderString.c_str();

    // Create shader
    auto glId = glCreateShader(shader->glType);

    // Set source code in the shader
    glShaderSource(glId, 1, &shaderChars, NULL);

    // Compile shader
    glCompileShader(glId);

    // Output a status log
    GLint logLength{};
    glGetShaderiv(glId, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar* log = (GLchar*)malloc(logLength);
        glGetShaderInfoLog(glId, logLength, &logLength, log);
        PJ::Log("Shader compile log: ", log);
        free(log);
    }

    // Check to see if the shader compiled.
    glGetShaderiv(glId, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        PJ::Log("ERROR. Failed to compile shader");
        return Failure();
    } else {
        shader->glId = glId;
        shader->isCompiled = true;
    }

    ResourceModel resource{ .info = info, .resource = shader };
    VectorList<ResourceModel> resources{ resource };

    return shader->isCompiled ? Result(Success(resources, {})) : Result(Failure());
}
