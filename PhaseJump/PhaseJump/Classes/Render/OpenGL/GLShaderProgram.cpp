#include "GLShaderProgram.h"
#include "GLHeaders.h"

using namespace PJ;

Map<String, SP<GLShaderProgram>> GLShaderProgram::registry;
VectorList<GLShaderProgram::Info> GLShaderProgram::Info::registry;

GLShaderProgram::~GLShaderProgram()
{
    if (glId) {
        glDeleteProgram(glId);
    }
}

void GLShaderProgram::Destroy()
{
    if (glId) {
        glDeleteProgram(glId);
        glId = 0;
    }
}

void GLShaderProgram::FlushShaders()
{
    vertexShader = nullptr;
    fragmentShader = nullptr;
}

/// Attach shaders. Use shared-ptr because there's no guarantee when we'll be done configuring the program
void GLShaderProgram::AttachShaders(SP<VertexGLShader> vertexShader, SP<FragmentGLShader> fragmentShader)
{
    if (nullptr == vertexShader || nullptr == fragmentShader || !vertexShader->isCompiled || !fragmentShader->isCompiled) {
        PJLog("ERROR. Invalid shaders for program.");
        return;
    }

    this->vertexShader = vertexShader;
    this->fragmentShader = fragmentShader;

    glId = glCreateProgram();
    glAttachShader(glId, vertexShader->glId);
    glAttachShader(glId, fragmentShader->glId);

    // Next steps:
    // - Bind any attribute locations for variables passed to shader
    // - Link the shader program.
}

/// Bind attributes in vertex shader (Vertex shader only)
/// You can either bind your own locations or use the defaults
void GLShaderProgram::BindAttributeLocation(GLuint index, const GLchar* name) {
    if (0 == glId) {
        PJLog("ERROR. Create program before binding attribute locations");
        return;
    }

    glBindAttribLocation(glId, index, name);
    attributeLocations[String(name)] = index;
}

/// Verify the validity of the GL shader program and return the status
GLint GLShaderProgram::Validate()
{
    GLint logLength;
    GLint status = 0;

    glValidateProgram(glId);
    glGetProgramiv(glId, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar*)malloc(logLength);
        glGetProgramInfoLog(glId, logLength, &logLength, log);
        PJLog("Program validate log:\n%s", log);
        free(log);
    }

    glGetProgramiv(glId, GL_VALIDATE_STATUS, &status);
    if (status == GL_FALSE) {
        PJLog("Failed to validate program %d", glId);
    }

    return status;
}

bool GLShaderProgram::Link()
{
    if (isLinked) { return true; }

    // Binding attribute locations for variables passed to shaders should
    // be done before linking (or you can just use the default locations)

    glLinkProgram(glId);

    GLint status;
    glGetProgramiv(glId, GL_LINK_STATUS, &status);
    if (GL_FALSE == status) {
        PJLog("ERROR. Failed to link program %d", glId);
        return false;
    }
    else {
        isLinked = true;
    }

    GLsizei bufferSize = 255;
    GLsizei length;
    GLint size;
    GLenum type;

    GLint count;

    // Must be called after program is linked
    glGetProgramiv(glId, GL_ACTIVE_ATTRIBUTES, &count);

    char name[bufferSize];
    for (int i = 0; i < count; i++) {
        glGetActiveAttrib(glId, (GLuint)i, bufferSize, &length, &size, &type, name);
        auto location = glGetAttribLocation(glId, name);
        attributeLocations[String(name)] = location;

        PJLog("Attribute #%d Type: %u Name: %s\n Index: %d", location, type, name, location);
    }

    glGetProgramiv(glId, GL_ACTIVE_UNIFORMS, &count);

    for (int i = 0; i < count; i++)
    {
        glGetActiveUniform(glId, (GLuint)i, bufferSize, &length, &size, &type, name);
        auto location = glGetUniformLocation(glId, name);
        uniformLocations[String(name)] = location;

        PJLog("Uniform #%d Type: %u Name: %s\n Index: %d", i, type, name, location);
    }

    return isLinked;
}

bool GLShaderProgram::Configure(SP<VertexGLShader> vertexShader, SP<FragmentGLShader> fragmentShader) {
    AttachShaders(vertexShader, fragmentShader);

    if (!Link()) {
        return false;
    }

    if (Validate() == GL_FALSE) {
        return false;
    }

    FlushShaders();

    return true;
}
