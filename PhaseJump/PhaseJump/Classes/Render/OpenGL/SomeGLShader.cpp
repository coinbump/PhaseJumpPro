#include "SomeGLShader.h"
#include "GLHeaders.h"

using namespace std;
using namespace PJ;

FragmentGLShader::FragmentGLShader() {
    glType = GL_FRAGMENT_SHADER;
}

VertexGLShader::VertexGLShader() {
    glType = GL_VERTEX_SHADER;
}

SomeGLShader::SomeGLShader() {}

SomeGLShader::~SomeGLShader() {
    GUARD(glId)
    glDeleteShader(glId);
}

void SomeGLShader::Destroy() {
    GUARD(glId)
    glDeleteShader(glId);
    glId = 0;
}
