#ifndef SpriteRenderer_h
#define SpriteRenderer_h

#include "SomeRenderer.h"
#include "SDLLoadTextureOperation.h"
#include "SDLRenderContext.h"
#include "SDLColorBuilder.h"
#include "RenderIntoModel.h"
#include <memory>
#include <SDL2/SDL_render.h>
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

            auto sdlRenderContext = std::dynamic_pointer_cast<SDLRenderContext>(model.renderContext);
            if (!sdlRenderContext) { return; }

            auto renderer = sdlRenderContext->renderer;

            SDL_FRect position;
            position.x = model.position.x();
            position.y = model.position.y();
            position.w = texture->size.x();
            position.h = texture->size.y();
            SDL_RenderCopyF(renderer, texture->texture, nullptr, &position);

            // TODO: temp code for testing mesh renders below
//            VectorList<SDL_Vertex> vertices;
//            VectorList<int> triangles;
//
//            Mesh mesh;
//            mesh.colors.Add(Color::redColor);
//            mesh.colors.Add(Color::redColor);
//            mesh.colors.Add(Color::blueColor);
//            mesh.colors.Add(Color::blueColor);
//
//            vertices.Add(newVertex(mesh, Vector2(0, 0), 0));
//            vertices.Add(newVertex(mesh, Vector2(100.0f, 0), 1));
//            vertices.Add(newVertex(mesh, Vector2(0, 100.0f), 2));
//            vertices.Add(newVertex(mesh, Vector2(100.0f, 100.0f), 3));
//
//            triangles.Add(0);
//            triangles.Add(1);
//            triangles.Add(2);
//            triangles.Add(1);
//            triangles.Add(3);
//            triangles.Add(2);
//
//            SDL_RenderGeometry(renderer,
//                               nullptr,
//                               &vertices[0], (int)vertices.size(),
//                               &triangles[0], (int)triangles.size());
        }

        // TODO: temp code
        SDL_Vertex newVertex(RenderMesh mesh, Vector2 position, int index);
    };
}

#endif
