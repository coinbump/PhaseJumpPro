#pragma once

#include "FilePath.h"
#include "GLHeaders.h"
#include "GLShader.h"
#include "ShaderProgram.h"
#include "UnorderedMap.h"
#include "VectorList.h"
#include <memory>

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/21/24
 */
namespace PJ {
    class BaseGLRenderEngine;

    /// Links  a vertex and a fragment shader
    class GLShaderProgram : public ShaderProgram {
    public:
        /// Used to register shader program info before the GL context is ready
        /// so the render engine can load the programs later
        struct Info {
            String id;
            FilePath vertexShaderPath;
            FilePath fragmentShaderPath;
        };

    private:
        /// Disable copy
        GLShaderProgram(GLShaderProgram const&);

    protected:
        SP<VertexGLShader> vertexShader;
        SP<FragmentGLShader> fragmentShader;

        GLuint glId{};
        bool isLinked{};

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

        bool Configure(SP<VertexGLShader> vertexShader, SP<FragmentGLShader> fragmentShader);

        /// @return Returns true if a vertex attribute exists for this id
        bool HasVertexAttribute(String id) {
            return attributeLocations.find(id) != attributeLocations.end();
        }

        /// @return Returns true if a uniform attribute exists for this id
        bool HasUniform(String id) {
            return uniformLocations.find(id) != uniformLocations.end();
        }

        // MARK: Manual setup

        /// Attaches the shaders to a shader program
        void AttachShaders(SP<VertexGLShader> vertexShader, SP<FragmentGLShader> fragmentShader);

        /// Bind attributes in vertex shader (Vertex shader only)
        /// You can either bind your own locations or use the defaults
        void BindAttributeLocation(GLuint index, const GLchar* name);

        bool Link();

        /// Verify the validity of the GL shader program and return the status
        GLint Validate();

        bool IsLinked() const {
            return isLinked;
        }
    };
} // namespace PJ
