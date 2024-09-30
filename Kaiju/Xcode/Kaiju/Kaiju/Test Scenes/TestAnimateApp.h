#pragma once

#if DEVELOPMENT
#include <PhaseJump-Dev/PhaseJump-Dev.h>
#else
#include <PhaseJump/PhaseJump.h>
#endif

using namespace PJ;

class TestAnimateApp : public Scene {
public:
    using Base = Scene;
    using TextureList = VectorList<SP<SomeTexture>>;

    TextureList textures;

    TestAnimateApp() {}

    // MARK: Scene

    void LoadInto(WorldNode& root) override {
        QB(root).Named("TestAnimateApp").OrthoStandard().OnDropFiles([](auto args) {
            AnimatedSpriteRenderer::TextureList textures;
            for (auto& filePath : std::get<1>(args).filePaths) {
                LoadResourcesModel loadResourcesModel;
                // FUTURE: need factory for load operations so this isn't bound to OpenGL
                ResourceInfo info{ "", filePath, "texture" };
                auto loadTexture = MAKE<SDLLoadGLTextureOperation>(info, loadResourcesModel);
                loadTexture->Run();

                auto successValue = loadTexture->result->SuccessValue();
                GUARD_CONTINUE(successValue)
                GUARD_CONTINUE(!IsEmpty(successValue->loadedResources))

                auto texture = DCAST<SomeTexture>(successValue->loadedResources[0].resource);
                GUARD_CONTINUE(texture)

                textures.push_back(texture);
            }

            QB(*std::get<0>(args).owner)
                .And("Animation")
                .With<AnimatedTextureRenderer>(textures)
                .Drag();
        });
    }
};
