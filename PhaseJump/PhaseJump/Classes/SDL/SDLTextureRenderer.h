#ifndef SpriteRenderer_h
#define SpriteRenderer_h

#include "SomeRenderer.h"
#include "SDLLoadTextureOperation.h"
#include "SDLRenderContext.h"
#include "SDLColorBuilder.h"
#include <memory>
#include <SDL2/SDL_render.h>
#include <iostream>

#include "Array.h"

namespace PJ {
    struct Mesh
    {
        Vector<Vector2> vertices;
        Vector<Color32> colors;
        Vector<Vector2> uvs;
    };

    class SDLTextureRenderer : public SomeRenderer
    {
    public:
        std::shared_ptr<SDLTexture> texture;

        SDLTextureRenderer(std::shared_ptr<SDLTexture> texture) : texture(texture)
        {
        }
        
        void RenderInto(std::shared_ptr<SomeRenderContext> renderContext) override {
            if (owner.expired() || nullptr == texture) { return; }

            auto node = owner.lock();

            auto sdlRenderContext = std::dynamic_pointer_cast<SDLRenderContext>(renderContext);
            if (!sdlRenderContext) { return; }

            auto renderer = sdlRenderContext->renderer;

            SDL_FRect position;
            position.x = node->transform->position.x();
            position.y = node->transform->position.y();
            position.w = texture->size.x();
            position.h = texture->size.y();
            SDL_RenderCopyF(renderer, texture->texture, nullptr, &position);

            // TODO: temp code for testing mesh renders below
//            Array<SDL_Vertex> vertices;
//            Array<int> triangles;
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
        SDL_Vertex newVertex(Mesh mesh, Vector2 position, int index);
    };
}

#endif
