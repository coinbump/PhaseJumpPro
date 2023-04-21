#ifndef PJGLTEXTURE_H
#define PJGLTEXTURE_H

#include "RenderTexture.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 4/21/23
 */
namespace PJ {
    class GLTexture {
    public:
        GLuint glId;
        Vector2Int size;

        GLTexture(GLuint glId, Vector2Int size) : glId(glId), size(size) {
        }

        virtual ~GLTexture() {
            if (glId > 0) {
                glDeleteTextures(1, &glId);
            }
        }

        operator RenderTexture() const {
            return RenderTexture(glId, size);
        }
    };
}

#endif
