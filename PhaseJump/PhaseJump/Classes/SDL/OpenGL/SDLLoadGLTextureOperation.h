#ifndef PJSDLLOADGLTEXTUREOPERATION_H
#define PJSDLLOADGLTEXTUREOPERATION_H

#include "FilePath.h"
#include "SomeOperation.h"
#include "SDLTexture.h"
#include <SDL2/SDL_render.h>
#include <SDL2_image/SDL_image.h>
#include "GLHeaders.h"
#include "Vector2.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 4/19/23
 */
namespace PJ {
    class GLTexture;
    
    enum class TextureMagnification {
        Nearest,
        Linear
    };

    /// Use SDL to load an OpenGL texture
    class SDLLoadGLTextureOperation : public SomeOperation {
    protected:
        FilePath path;

    public:
        using Base = SomeOperation;

        std::shared_ptr<GLTexture> texture;
        TextureMagnification textureMagnification;

        SDLLoadGLTextureOperation(FilePath path, TextureMagnification textureMagnification);

        void GoInternal() override;
    };
}

#endif
