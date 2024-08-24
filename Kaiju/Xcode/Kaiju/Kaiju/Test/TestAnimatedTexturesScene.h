#pragma once

#include <PhaseJump/PhaseJump.h>

using namespace PJ;

class TestAnimatedTexturesScene : public Scene {
public:
    using Base = Scene;
    using TextureList = VectorList<SP<SomeTexture>>;

    TextureList textures;

    TestAnimatedTexturesScene() {
    }

    // TODO:
//    void ProcessUIEvents(List<SP<SomeUIEvent>> const& uiEvents) override {
//        Base::ProcessUIEvents(uiEvents);
//
//        for (auto& event : uiEvents) {
//            auto dropFileEvent = DCAST<DropFilesUIEvent>(event);
//            if (dropFileEvent) {
//                PJLog("Drop File");
//                RemoveAllNodes();
//
//                auto camera = SCAST<SomeCamera>(MAKE<OrthoCamera>());
//
//                auto cameraNode = MAKE<WorldNode>();
//                cameraNode->Add(camera);
//                auto raycaster = std::make_shared<SimpleRaycaster2D>();
//                cameraNode->Add(raycaster);
//
//                root->AddEdge(cameraNode);
//
//                VectorList<SP<SomeTexture>> glTextures;
//                for (auto& filePath : dropFileEvent->filePaths) {
//                    auto loadTexture = MAKE<SDLLoadGLTextureOperation>(filePath, TextureMagnification::Linear);
//                    loadTexture->Run();
//                    auto textures = loadTexture->result->SuccessValue();
//                    if (textures.size() > 0) {
//                        auto texture = SCAST<GLTexture>(textures[0].resource);
//                        glTextures.Add(texture);
//                    }
//                }
//
//                TestAnimatedTexturesScene::TextureList textures2{glTextures};
//                TestAnimatedTexturesScene scene(textures2);
//                scene.LoadInto(*this);
//                continue;
//            }
//
//            auto pointerDownEvent = DCAST<PointerDownUIEvent>(event);
//            if (pointerDownEvent) {
//                cout << "Pointer down at: " << pointerDownEvent->pressPosition.x << ", " << pointerDownEvent->pressPosition.y << "\n";
//            }
//        }
//    }

    void LoadInto(WorldNode& root) {
        root.name = "TestAnimatedTexturesScene";

        auto camera = SCAST<SomeCamera>(MAKE<OrthoCamera>());
        auto cameraNode = MAKE<WorldNode>();
        cameraNode->Add(camera);
        root.Add(cameraNode);

    }
};
