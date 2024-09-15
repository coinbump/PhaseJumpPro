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
    
    TestAnimateApp() {
        signalHandlers[SignalId::DropFiles] = [](auto& component, auto& signal) {
            auto event = static_cast<DropFilesUIEvent const*>(&signal);
            AnimatedSpriteRenderer::TextureList textures;
            for (auto& filePath : event->filePaths) {
                LoadResourcesModel loadResourcesModel;
                // FUTURE: need factory for load operations so this isn't bound to OpenGL
                ResourceInfo info{"", filePath, "texture"};
                auto loadTexture = MAKE<SDLLoadGLTextureOperation>(info, loadResourcesModel);
                loadTexture->Run();

                auto successValue = loadTexture->result->SuccessValue();
                GUARD(successValue)
                GUARD(!IsEmpty(successValue->loadedResources))

                auto texture = SCAST<SomeTexture>(successValue->loadedResources[0].resource);
                textures.push_back(texture);
            }

            QB(*component.owner)
                .And("Animation")
                .With<AnimatedTextureRenderer>(textures)
                .Drag();
        };
    }

    // MARK: Scene

    void LoadInto(WorldNode& root) override {
        QB(root)
            .Named("TestAnimateApp")
            .With<OrthoCamera>()
            .With<SimpleRaycaster2D>();
    }
};
