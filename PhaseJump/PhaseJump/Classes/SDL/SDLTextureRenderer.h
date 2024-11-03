#ifndef SpriteRenderer_h
#define SpriteRenderer_h

#include "RenderContextModel.h"
#include "SDLColorBuilder.h"
#include "SDLGLRenderContext.h"
#include "SDLLoadSDLTextureOperation.h"
#include "SomeRenderer.h"
#include <memory>
#include <SDL3/SDL_render.h>
// #include <SDL3/SDL_render_gl.h>
#include "VectorList.h"
#include <iostream>

// CODE REVIEW: ?/23
namespace PJ {
    class Mesh;

    //    class SDLTextureRenderer : public SomeRenderer
    //    {
    //    public:
    //        SP<SDLTexture> texture;
    //
    //        SDLTextureRenderer(SP<SDLTexture> texture) : texture(texture)
    //        {
    //        }
    //
    //        void RenderInto(RenderContextModel const& contextModel) override {
    //            if (owner.expired() || nullptr == texture) { return; }
    //
    //            auto node = owner.lock();
    //
    //            float width, height;
    ////            SDL_GL_BindTexture(texture, &width, &height);
    //
    //            // SDL Renderer code (when not using OpenGL). Keep for
    //            reference.
    ////            auto sdlRenderContext =
    /// DCAST<SDLRenderContext>(model.renderContext); /            if
    ///(!sdlRenderContext) { return; }
    ////
    ////            auto renderer = sdlRenderContext->renderer;
    ////
    ////            SDL_FRect position;
    ////            position.x = model.position.x;
    ////            position.y = model.position.y;
    ////            position.w = texture->size.x;
    ////            position.h = texture->size.y;
    ////            SDL_RenderTexture(renderer, texture->texture, nullptr,
    ///&position);
    //        }
    //    };
} // namespace PJ

#endif
