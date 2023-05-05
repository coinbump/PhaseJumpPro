#ifndef PJSDLLOADGLTEXTUREOPERATION_H
#define PJSDLLOADGLTEXTUREOPERATION_H

#include "FilePath.h"
#include "SomeOperation.h"
#include "SDLTexture.h"
#include "SomeLoadResourcesOperation.h"
#include <SDL2/SDL_render.h>
#include <SDL2_image/SDL_image.h>
#include "GLHeaders.h"
#include "Vector2.h"
#include "RenderTypes.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 4/19/23
 */
namespace PJ {
    class GLTexture;

    /// Use SDL to load an OpenGL texture
    class SDLLoadGLTextureOperation : public SomeLoadResourcesOperation {
    public:
        using Base = SomeOperation;

        TextureMagnification textureMagnification = TextureMagnification::Linear;

        SDLLoadGLTextureOperation() {
        }

        SDLLoadGLTextureOperation(FilePath filePath, TextureMagnification textureMagnification);

        void Run() override;
    };
}

#endif
