#ifndef PJGLSHADERPROGRAM_H
#define PJGLSHADERPROGRAM_H

#include "FilePath.h"
#include "GLHeaders.h"
#include "SomeGLShader.h"
#include "SomeShaderProgram.h"
#include "UnorderedMap.h"
#include "VectorList.h"
#include <memory>

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 4/13/23
 */
namespace PJ {
    class SomeGLRenderEngine;

    /// Links  a vertex and a fragment shader
    class GLShaderProgram : public SomeShaderProgram {
    public:
        /// Used to register shader program info before the GL context is ready
        /// so the render engine can load the programs later
        struct Info {
            String id;
            FilePath vertexShaderPath;
            FilePath fragmentShaderPath;

            static VectorList<Info> registry;
        };

    private:
        /// Disable copy
        GLShaderProgram(GLShaderProgram const&);

    protected:
        // TODO: SP-audit
        SP<VertexGLShader> vertexShader;
        SP<FragmentGLShader> fragmentShader;

        GLuint glId = 0;
        bool isLinked = false;

    public:
        /// Map of attribute names to attribute location index
        UnorderedMap<String, GLuint> attributeLocations;

        /// Map of uniform names to uniform location index
        UnorderedMap<String, GLuint> uniformLocations;

        GLShaderProgram() {}

        virtual ~GLShaderProgram();

        GLuint GLId() const {
            return glId;
        }

        void Destroy();
        void FlushShaders();

        // Full setup (you should use this)
        bool Configure(SP<VertexGLShader> vertexShader, SP<FragmentGLShader> fragmentShader);

        bool HasVertexAttribute(String id) {
            return attributeLocations.find(id) != attributeLocations.end();
        }

        bool HasUniform(String id) {
            return uniformLocations.find(id) != uniformLocations.end();
        }

        // Manual setup
        void AttachShaders(SP<VertexGLShader> vertexShader, SP<FragmentGLShader> fragmentShader);
        void BindAttributeLocation(GLuint index, const GLchar* name);
        bool Link();
        GLint Validate();

        bool IsLinked() const {
            return isLinked;
        }
    };
} // namespace PJ

#endif
