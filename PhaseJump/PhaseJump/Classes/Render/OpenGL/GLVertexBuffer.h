#ifndef PJGLVERTEXBUFFER_H
#define PJGLVERTEXBUFFER_H

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 4/16/23
 */

#include "_Map.h"

namespace PJ {
    class GLSomeBuffer {
    public:
        GLuint glId = 0;

        GLSomeBuffer() {
        }

        GLSomeBuffer(GLuint glId) : glId(glId) {
        }

        virtual ~GLSomeBuffer() {
            if (glId) {
                glDeleteBuffers(1, &glId);
            }
        }
    };

    /**
     A vertex buffer (VBO) stores vertices, colors, etc. in a `GL_ARRAY_BUFFER` for access by the GPU.
     Vertices can be rendered via an IBO (Index Buffer Object), which specifies the indices into the VBO.
     A VBO can contain render components for multiple objects, to make batching draw calls easier

     Usage: use pointers or the copied object will delete the buffer
     */
    class GLVertexBuffer : public GLSomeBuffer {
    public:
        struct Attribute {
            uint32_t offset;
            GLenum glType;
            bool normalize;
        };

        Map<String, Attribute> attributes;

        GLVertexBuffer() {
        }

        GLVertexBuffer(GLuint glId) : GLSomeBuffer(glId) {
        }
    };

    /**
     An index buffer (IBO/EBO) stores indices in a `GL_ELEMENT_ARRAY_BUFFER` for access by the GPU.
     Specifies vertex indices in a Vertex Buffer (VBO)

     Usage: use pointers or the copied object will delete the buffer
     */
    class GLIndexBuffer : public GLSomeBuffer {
    public:
        GLIndexBuffer() {
        }

        GLIndexBuffer(GLuint glId) : GLSomeBuffer(glId) {
        }
    };
}

#endif
