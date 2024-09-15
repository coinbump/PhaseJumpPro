#pragma once

#include "FilePath.h"
#include "GLHeaders.h"
#include "RenderTypes.h"
#include "SDLTexture.h"
#include "SomeLoadResourcesOperation.h"
#include "SomeOperation.h"
#include "Vector2.h"
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>

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
        using Base = SomeLoadResourcesOperation;

        // TODO: Does it make sense to put this here??
        String textureMagnification = TextureMagnification::Linear;

        SDLLoadGLTextureOperation(ResourceInfo info, LoadResourcesModel& loadResourcesModel) :
            Base(info, loadResourcesModel) {}

        // MARK: SomeLoadResourcesOperation

        Result LoadResources() override;
    };
} // namespace PJ
