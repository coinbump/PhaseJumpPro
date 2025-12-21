#include "GLHeaders.h"
#include "GLShader.h"

using namespace std;
using namespace PJ;

FragmentGLShader::FragmentGLShader() {
    glType = GL_FRAGMENT_SHADER;
}

VertexGLShader::VertexGLShader() {
    glType = GL_VERTEX_SHADER;
}

GLShader::GLShader() {}

GLShader::~GLShader() {
    GUARD(glId)
    glDeleteShader(glId);
}

void GLShader::Destroy() {
    GUARD(glId)
    glDeleteShader(glId);
    glId = 0;
}
