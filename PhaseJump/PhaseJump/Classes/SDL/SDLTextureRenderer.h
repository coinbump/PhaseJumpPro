#ifndef SpriteRenderer_h
#define SpriteRenderer_h

#include "SomeRenderer.h"
#include "SDLLoadTextureOperation.h"
#include "SDLRenderContext.h"
#include "SDLColorBuilder.h"
#include "RenderIntoModel.h"
#include <memory>
#include <SDL2/SDL_render.h>
//#include <SDL2/SDL_render_gl.h>
#include <iostream>

#include "VectorList.h"

namespace PJ {
    class RenderMesh;
    
    class SDLTextureRenderer : public SomeRenderer
    {
    public:
        std::shared_ptr<SDLTexture> texture;

        SDLTextureRenderer(std::shared_ptr<SDLTexture> texture) : texture(texture)
        {
        }
        
        void RenderInto(RenderIntoModel model) override {
            if (owner.expired() || nullptr == texture) { return; }

            auto node = owner.lock();

            float width, height;
//            SDL_GL_BindTexture(texture, &width, &height);

            // SDL Renderer code (when not using OpenGL). Keep for reference.
//            auto sdlRenderContext = std::dynamic_pointer_cast<SDLRenderContext>(model.renderContext);
//            if (!sdlRenderContext) { return; }
//
//            auto renderer = sdlRenderContext->renderer;
//
//            SDL_FRect position;
//            position.x = model.position.x;
//            position.y = model.position.y;
//            position.w = texture->size.x;
//            position.h = texture->size.y;
//            SDL_RenderCopyF(renderer, texture->texture, nullptr, &position);
        }
    };
}

#endif
