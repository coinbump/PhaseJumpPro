#pragma once

#include <PhaseJump/PhaseJump.h>
#include "TestGradientsScene.h"
#include "TestMeshPathScene.h"
#include "TestTextureScene.h"
#include "TestSlicedTextureScene.h"
#include "TestAnimatedTexturesScene.h"
#include "TestAudioScene.h"
#include "TestEmittersScene.h"

using namespace PJ;

class KaijuSceneClass : public TypeClass<Scene> {
public:
    KaijuSceneClass(String id, String name, FactoryFunc allocator) : TypeClass<Scene>(id, allocator) {
        core.name = name;
    }
};

class KaijuImGuiRenderer : public SomeRenderer {
public:
    List<SP<KaijuSceneClass>> sceneClasses;
    bool isWindowConfigured = false;
    bool isToolActive = false;

    KaijuImGuiRenderer() {
        sceneClasses.Add(MAKE<KaijuSceneClass>("test.texture", "Test Texture", []() { return MAKE<TestTextureScene>(); }));
        sceneClasses.Add(MAKE<KaijuSceneClass>("test.slicedTexture", "Test Sliced Texture", []() { return MAKE<TestSlicedTextureScene>(); }));
        sceneClasses.Add(MAKE<KaijuSceneClass>("test.gradients", "Test Gradients", []() { return MAKE<TestGradientsScene>(); }));
        sceneClasses.Add(MAKE<KaijuSceneClass>("test.meshPath", "Test Mesh Path", []() { return MAKE<TestMeshPathScene>(); }));
        sceneClasses.Add(MAKE<KaijuSceneClass>("test.audio", "Test Audio", []() { return MAKE<TestAudioScene>(); }));
        sceneClasses.Add(MAKE<KaijuSceneClass>("test.emitters", "Test Emitters", []() { return MAKE<TestEmittersScene>(); }));
    }

    // MARK: SomeRenderer

    VectorList<RenderModel> MakeRenderModels(RenderIntoModel const& model) override {
        VectorList<RenderModel> result;
        return result;
    }

    void RenderInto(RenderIntoModel const& model) override {
        GUARD(owner)
        auto world = owner->world;
        GUARD(world)

        if (!isWindowConfigured) {
            isWindowConfigured = true;

            ImGui::SetNextWindowSize(ImVec2(300, 500));
            ImGui::SetNextWindowPos(ImVec2(20, 20));
        }

        ImGui::Begin("Kaiju", &isToolActive, ImGuiWindowFlags_MenuBar);

        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Scenes");

        // TODO: this is very, very slow :(
//        GraphNodeTool<> nodeTool;
//        auto graph = nodeTool.CollectBreadthFirstGraph(world->root);
//
//        // TODO: cache this count somewhere + fps and show both
//        String total = String("Node Count: ") + String((int)graph.size());
//        ImGui::TextColored(ImVec4(1, 1, 1, 1), "%s", total.c_str());
        
        int fps = world->renderStats.SafeValue<int>("fps");
        String fpsString(fps);
        String fpsTitle = String("FPS: ") + fpsString;
        ImGui::TextColored(fps < 30 ? ImVec4(1, 0, 0, 1) : ImVec4(0, 1, 0, 1), "%s", fpsTitle.c_str());

        int drawCount = world->renderStats.SafeValue<int>("draw.count");
        String drawCountString(drawCount);
        String drawCountTitle = String("Draw Count: ") + drawCountString;
        ImGui::TextColored(ImVec4(1, 1, 1, 1), "%s", drawCountTitle.c_str());

        ImGui::BeginChild("Scrolling");
        for (auto& sceneClass : sceneClasses) {
            auto name = sceneClass->core.name;

            // ImGui requires that UI elements have names (unless you specify ##)
            GUARD_CONTINUE(name.size() > 0)

            if (ImGui::Button(name.c_str())) {
                auto scene = sceneClass->Make();
                if (scene) {
                    SP<WorldNode> sharedOwner = SCAST<WorldNode>(owner->shared_from_this());

                    world->RemoveAllSystems();
                    world->RemoveAllNodes();

                    auto sceneNode = MAKE<WorldNode>();
                    sceneNode->Add(scene);
                    world->Add(sceneNode);

                    world->Add(sharedOwner);
                }
            }
        }
        ImGui::EndChild();
        ImGui::End();
    }
};
