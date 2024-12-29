#pragma once

#include "SDKIncludes.h"

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
            for (auto& filePath : args.event.filePaths) {
                ResourceRepositoryModel repoModel;
                // TODO: use ResourceRepository here
                ResourceInfo info{ .filePath = filePath, .type = ResourceType::Texture };
                auto loadTexture = MAKE<SDLLoadGLTextureOperation>(info, repoModel);
                loadTexture->Run();

                auto successValue = loadTexture->result->SuccessValue();
                GUARD_CONTINUE(successValue)
                GUARD_CONTINUE(!IsEmpty(successValue->resources))

                auto texture = DCAST<SomeTexture>(successValue->resources[0].resource);
                GUARD_CONTINUE(texture)

                textures.push_back(texture);
            }

            QB(*args.component.owner)
                .And("Animation")
                .With<AnimatedTextureRenderer>(textures)
                .Drag();
        });
    }
};
