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
 CODE REVIEW: 12/21/24
 */
namespace PJ {
    class GLTexture;

    /// Uses SDL to load an OpenGL texture
    class SDLLoadGLTextureOperation : public SomeLoadResourcesOperation {
    public:
        using Base = SomeLoadResourcesOperation;

        String textureMagnification = TextureMagnify::Linear;

        SDLLoadGLTextureOperation(ResourceInfo info, ResourceRepositoryModel& repoModel) :
            Base(info, repoModel) {}

        // MARK: SomeLoadResourcesOperation

        Result LoadResources() override;
    };
} // namespace PJ
